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

#include "2DPrimitives/Constructs/MovableObject.h"
#include "Graphics/Renderer/RenderableComponents/Renderable2D.h"
#include "Graphics/Renderer/RenderableComponents/RenderableTextureModule.h"

#include "Physics/Physics.h"
#include "Physics/PhysicsComponent2D.h"
#include "RenderableComponents/RenderablePhysicsComponent2D.h"

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

				//Start If A Movable Object Is Detected Inside The Rendererable Do The Following Steps//
				if(dynamic_cast <Ermine::MovableObject*>(i))
				{
					i->Bind();
					
					MovableObject* MovObj = dynamic_cast<MovableObject*>(i);
					i->GetMaterialBeingUsed()->GetShader()->UniformMat4(std::string("ModelMatrix"), (MovObj)->GetModelMatrix());
				}
				//Ended If A Movable Object Is Detected Inside The Rendererable Do The Following Steps//

				//Start If A PhysicsComponent2D is detected inside the rendererable do the following steps//
				if (dynamic_cast <Ermine::RenderablePhysicsComponent2D*>(i))
				{
					i->Bind();

					RenderablePhysicsComponent2D* PhysicsObject = dynamic_cast<RenderablePhysicsComponent2D*>(i);
					i->GetMaterialBeingUsed()->GetShader()->UniformMat4(std::string("ModelMatrix"), (PhysicsObject)->GetTranslationMatrix());
				}
				//Ended If A PhysicsComponent2D is detected inside the rendererable do the following steps//

				//Start If A Renderable Texture module Is detected Inside the renderable do the following steps//
				if (dynamic_cast <Ermine::RenderableTextureModule*>(i))
				{
					i->Bind();

					Ermine::RenderableTextureModule* Ptr = (RenderableTextureModule*)i;
					std::vector<int> TextureArray = Ptr->BindTexturesContained();
					TextureArray.resize(16, 0);

					i->GetMaterialBeingUsed()->GetShader()->UniformNi(std::string("Sampler2DArray"),TextureArray);
				}
				//Ended If A Renderable Texture module Is detected Inside the renderable do the following steps//

				//Start These Steps Are To Be Done on All Kinds Of Renderables..//
				i->GetMaterialBeingUsed()->GetShader()->UniformMat4(std::string("ProjectionViewMatrix"), Renderer->ProjectionViewMatrix);
				glDrawElements(GL_TRIANGLES, i->GetVertexArray()->GetIndexBufferLength(), GL_UNSIGNED_INT, 0);
				//Ended These Steps Are To Be Done on All Kinds Of Renderables..//
			}
		}
	}
}
