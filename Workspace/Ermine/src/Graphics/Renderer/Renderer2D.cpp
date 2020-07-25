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

#pragma region StaticDeclarationRegion

std::once_flag Ermine::Renderer2D::InitializationFlag;
Ermine::Renderer2D* Ermine::Renderer2D::GlobalRenderer2DObj;

#pragma endregion StaticDeclarationRegion

namespace Ermine
{
	Renderer2D::Renderer2D()
		:
		RendererLayerStack(std::string("Renderer2DLayerStack"))
	{
		/*Actor2DShader = new Shader(std::filesystem::path("Shader/Vertex/Renderer2DActor2DVertex.vert"),
								   std::filesystem::path("Shader/Fragment/Renderer2DActor2DFragment.frag"));

		TileMapShader = new Shader(std::filesystem::path("Shader/Vertex/TileMapVertexShader.vert"),
								   std::filesystem::path("Shader/Fragment/TileMapFragmentShader.frag"));*/
	}
	Renderer2D::~Renderer2D()
	{
		//delete Actor2DShader;
		//delete TileMapShader;
	}


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

	/*void Renderer2D::DrawActor2D(Actor2D* Act)
	{
		auto Renderer = Ermine::Renderer2D::Get();
		Renderer->StowedActors.emplace_back(Act);
	}

	void Renderer2D::DrawTileMap(TileMap* Tm)
	{
		auto Renderer = Ermine::Renderer2D::Get();
		Renderer->StowedTileMaps.emplace_back(Tm);
	}*/

	void Renderer2D::EndScene()
	{
		auto Renderer = Ermine::Renderer2D::Get();
		assert(Renderer->SceneBegin); //Note Scene Must Have Begun To End Otherwise It cannot Be Ended..
		Renderer->SceneBegin = false;

		Renderer->DrawingHelper();
		
		Renderer->RendererLayerStack.Clear();
		//DrawActorsHelper();
		//Renderer->DrawTileMapHelper();
	}

	/*void Renderer2D::ClearStowedActors()
	{
		StowedActors.clear();
	}*/

	void Renderer2D::DrawingHelper()
	{
		//Getting The Texture Cache As It May Prove Helpful..
		auto TextureCacheGlobal = Ermine::GlobalTextureCache::Get();
		auto Renderer = Ermine::Renderer2D::Get();

		for (auto layer : RendererLayerStack.AllLayersAssociated)
		{
			for (Renderable2D* i : layer->Renderables)
			{
				i->Bind();

				if(i->GetType() == Renderable2DType::ACTOR2D)
				{
					Actor2D* Actor = (Actor2D*)i;
					i->GetMaterialBeingUsed()->GetShader()->UniformMat4(std::string("ModelMatrix"), ((Actor2D*)i)->GetModelMatrix());
					int BindSlot = GlobalTextureCache::Get()->Bind(((Actor2D*)i)->GetSprite()->GetTexture());
					i->GetMaterialBeingUsed()->GetShader()->Uniformi(std::string("texture1"), BindSlot);
				}
				


				i->GetMaterialBeingUsed()->GetShader()->UniformMat4(std::string("ProjectionViewMatrix"), Renderer->ProjectionViewMatrix);
				glDrawElements(GL_TRIANGLES, i->GetVertexArray()->GetIndexBufferLength(), GL_UNSIGNED_INT, 0);
			}
		}
	}


	/*void Renderer2D::DrawActorsHelper()
	{
		auto Renderer = Ermine::Renderer2D::Get();

		Renderer->Actor2DShader->Bind();

		for (auto i : Renderer->StowedActors)
		{
			VertexArray Vao(i->GetModelSpaceCoordinates(), Actor2D::GetModelSpaceIndices());
			Vao.Bind();

			static std::vector<VertexAttribPointerSpecification> Spec = {
				{3,GL_FLOAT,false},
				{3,GL_FLOAT,false},
				{2,GL_FLOAT,false}
			};
			Vao.SetVertexAttribArray(Spec);

			Vao.Bind();

			glm::mat4 ModelMatrix = i->GetModelMatrix();
			Renderer->Actor2DShader->UniformMat4(std::string("ModelMatrix"), ModelMatrix);
			Renderer->Actor2DShader->UniformMat4(std::string("ProjectionViewMatrix"), Renderer->ProjectionViewMatrix);

			int BindSlot = GlobalTextureCache::Get()->Bind(i->GetSprite()->GetTexture());
			Renderer->Actor2DShader->Uniformi(std::string("texture1"), BindSlot);

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		Renderer->StowedActors.clear();
	}

	void Renderer2D::DrawTileMapHelper()
	{
		auto Renderer = Ermine::Renderer2D::Get();

		std::vector<Ermine::VertexArray> VaoArray;
		std::vector<std::unordered_map<std::filesystem::path, float>> TextureMappingCache;

		for (auto map : Renderer->StowedTileMaps)
		{
			auto DrawPrimitive = map->RendererFriendlyDrawable;

			for (int i = 0; i < DrawPrimitive.GetNumberOfLayersStored(); i++)
			{
				Ermine::VertexArray* Vao = DrawPrimitive.GetVao(i);
				std::unordered_map<std::filesystem::path, float> TextureToNumberInVaoMapper = DrawPrimitive.GetTextureToNumberCache(i);

				Renderer->TileMapShader->Bind();
				Vao->Bind();

				Vao->SetVertexAttribArray({
				{3,GL_FLOAT,false},
				{2,GL_FLOAT,false},
				{1,GL_FLOAT,false}
				});

				static glm::mat4 ProjectionViewMatrix = glm::ortho<float>(0.0f,((float)Ermine::GetScreenWidth()),((float)Ermine::GetScreenHeight()), 0.0f, -5.0f, 5.0f);

				Renderer->TileMapShader->UniformMat4(std::string("ProjectionViewMatrix"), ProjectionViewMatrix); //Here See That The Projection MAtrix Is Akin To Screen Coordinates..

				auto TextureCacheGlobal = Ermine::GlobalTextureCache::Get();
				std::vector<float> TextureMappingUnits;
				TextureMappingUnits.resize(16);

				for (int i = 0; i < 16; i++)
					TextureMappingUnits[i] = 0.0f;

				for (auto i = TextureToNumberInVaoMapper.begin(); i != TextureToNumberInVaoMapper.end(); i++)
				{
					std::shared_ptr<Texture> Tex = TextureCacheGlobal->GetTextureFromFile(i->first);
					int BoundSlot = TextureCacheGlobal->Bind(Tex);

					TextureMappingUnits[(int)i->second] = BoundSlot;
				}
				
				Renderer->TileMapShader->UniformNf(std::string("Sampler2DArray"),TextureMappingUnits);

				GLCall(glDrawElements(GL_TRIANGLES, Vao->GetIndexBufferLength(), GL_UNSIGNED_INT, 0));
			}
		}
		Renderer->StowedTileMaps.clear();
	}*/
}
