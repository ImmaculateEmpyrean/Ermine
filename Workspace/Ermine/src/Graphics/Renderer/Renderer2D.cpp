#include "stdafx.h"
#include "Renderer2D.h"

//This Is an OpenGL Specific Renderer So Not a Big Deal..
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "OpenGLErrorChecker.h"

#include "RendererPrimitives/VertexArray.h"
#include "RendererPrimitives/VertexBuffer.h"
#include "RendererPrimitives/IndexBuffer.h"

#include "Graphics/Renderer/MaterialSystem/Shader.h"
#include "Graphics/Renderer/MaterialSystem/Texture.h"

#include "imgui.h"

#include "2DPrimitives/Constructs/MovableObject.h"

#pragma region StaticDeclarationRegion

std::once_flag Ermine::Renderer2D::InitializationFlag;
Ermine::Renderer2D* Ermine::Renderer2D::GlobalRenderer2DObj;

#pragma endregion StaticDeclarationRegion

namespace Ermine
{
	Renderer2D::Renderer2D()
		:
		RendererLayerStack(std::string("Renderer2DLayerStack"))
	{}

	Renderer2D::~Renderer2D()
	{}


	Renderer2D* Renderer2D::Get()
	{
		std::call_once(InitializationFlag, []() {
			GlobalRenderer2DObj = new Renderer2D();
		});

		return GlobalRenderer2DObj;
	}


	void Renderer2D::BeginScene(glm::mat4 Camera, glm::mat4 ProjectionMatrix)
	{
		auto Renderer = Ermine::Renderer2D::Get();
		
		Renderer->CameraMatrix = Camera;
		Renderer->ProjectionMatrix = ProjectionMatrix;

		Renderer->ProjectionViewMatrix = ProjectionMatrix * Camera;

		Renderer->SceneBegin = true;
	}

	void Renderer2D::SubmitLayer(LayerStackLayer layer)
	{
		auto Renderer = Ermine::Renderer2D::Get();
		Renderer->RendererLayerStack.PushLayerOnTheBackOfTheStack(std::make_unique<LayerStackLayer>(layer));
	}

	void Renderer2D::SubmitLayer(LayerStackLayer layer, int index)
	{
		auto Renderer = Ermine::Renderer2D::Get();
		Renderer->RendererLayerStack.PushLayerOntoStackAtPosition(std::make_unique<LayerStackLayer>(layer),index);
	}

	void Renderer2D::ReplaceLayerStackWithStack(LayerStack layerstack)
	{
		auto Renderer = Renderer2D::Get();

		Renderer->RendererLayerStack.Clear();
		Renderer->RendererLayerStack = layerstack;
	}

	void Renderer2D::EndScene()
	{
		auto Renderer = Ermine::Renderer2D::Get();
		assert(Renderer->SceneBegin); //Note Scene Must Have Begun To End Otherwise It cannot Be Ended..
		Renderer->SceneBegin = false;

		Renderer->DrawingRoutine();
		
		Renderer->RendererLayerStack.Clear();
	}

	void Renderer2D::DrawingRoutine()
	{
		//Getting The Texture Cache As It May Prove Helpful..
		auto TextureCacheGlobal = Ermine::GlobalTextureCache::Get();
		auto Renderer = Ermine::Renderer2D::Get();

		for (auto layer : RendererLayerStack.AllLayersAssociated)
		{
			for (Renderable2D* i : layer->Renderables)
			{
				i->Bind();

				if(dynamic_cast <Ermine::MovableObject*>(i))
				{
					i->Bind();
					
					MovableObject* MovObj = dynamic_cast<MovableObject*>(i);
					i->GetMaterialBeingUsed()->GetShader()->UniformMat4(std::string("ModelMatrix"), (MovObj)->GetModelMatrix());
				}
				
				if (dynamic_cast <Ermine::RenderableTextureModule*>(i))
				{
					i->Bind();

					Ermine::RenderableTextureModule* Ptr = (RenderableTextureModule*)i;
					std::vector<float> TextureArray = Ptr->BindTexturesContained();
					
					TextureArray.resize(16, -1);

					TextureArray[0] = 0.0f;

					i->GetMaterialBeingUsed()->GetShader()->UniformNf(std::string("Sampler2DArray"),TextureArray);
				}

				if (i->GetType() == Renderable2DType::TileMap)
				{
					TileMapLayerRenderable* LayerRenderable = (TileMapLayerRenderable*)i;

					std::vector<float> TextureMappingUnits;
					TextureMappingUnits.resize(16);

					for (int i = 0; i < 16; i++)
						TextureMappingUnits[i] = 0.0f;

					for (auto Cask = LayerRenderable->TexturesAndNumbers.begin(); Cask != LayerRenderable->TexturesAndNumbers.end(); Cask++)
					{
						std::shared_ptr<Texture> Tex = TextureCacheGlobal->GetTextureFromFile(Cask->first);
						int BoundSlot = TextureCacheGlobal->Bind(Tex);

						TextureMappingUnits[(int)Cask->second] = BoundSlot;
					}
					i->Mat->GetShader()->UniformNf(std::string("Sampler2DArray"), TextureMappingUnits);
				}

				i->GetMaterialBeingUsed()->GetShader()->UniformMat4(std::string("ProjectionViewMatrix"), Renderer->ProjectionViewMatrix);
				glDrawElements(GL_TRIANGLES, i->GetVertexArray()->GetIndexBufferLength(), GL_UNSIGNED_INT, 0);
			}
		}
	}
}
