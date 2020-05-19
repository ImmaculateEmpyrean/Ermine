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
	Renderer2D::~Renderer2D()
	{

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

	void Renderer2D::DrawActor2D(Actor2D* Act)
	{
		auto Renderer = Ermine::Renderer2D::Get();
		Renderer->StowedActors.emplace_back(Act);
	}

	void Renderer2D::DrawTileMap(TileMap* Tm)
	{
		auto Renderer = Ermine::Renderer2D::Get();
		Renderer->StowedTileMaps.emplace_back(Tm);
	}

	void Renderer2D::EndScene()
	{
		auto Renderer = Ermine::Renderer2D::Get();
		assert(Renderer->SceneBegin); //Note Scene Must Have Begun To End Otherwise It cannot Be Ended..
		Renderer->SceneBegin = false;

		DrawActorsHelper();
		DrawTileMapHelper();
	}

	void Renderer2D::SetNumberOfGridsOnScreen(glm::vec<2, int> NumberOnXAndY)
	{
		auto Renderer = Ermine::Renderer2D::Get();

		if (NumberOnXAndY.x == Renderer->NumberOfGridsInXOnScreen)
		{
			if (NumberOnXAndY.y == Renderer->NumberOfGridsInYOnScreen)
			{
				return;
			}
			else Renderer->RenderPrimitiveCache.clear();
		}
		else Renderer->RenderPrimitiveCache.clear();
	}


	void Renderer2D::ClearStowedActors()
	{
		StowedActors.clear();
	}


	void Renderer2D::DrawActorsHelper()
	{
		auto Renderer = Ermine::Renderer2D::Get();

		static Shader* shd = new Shader(std::filesystem::path("Shader/Vertex/Renderer2DActor2DVertex.vert"),
			std::filesystem::path("Shader/Fragment/Renderer2DActor2DFragment.frag"));
		shd->Bind();

		for (auto i : Renderer->StowedActors)
		{
			VertexArray Vao(i->GetModelSpaceCoordinates(), Actor2D::GetModelSpaceIndices());
			Vao.Bind();

			std::vector<VertexAttribPointerSpecification> Spec = {
				{3,GL_FLOAT,false},
				{3,GL_FLOAT,false},
				{2,GL_FLOAT,false}
			};
			Vao.SetVertexAttribArray(Spec);

			Vao.Bind();

			glm::mat4 ModelMatrix = i->GetModelMatrix();
			shd->UniformMat4(std::string("ModelMatrix"), ModelMatrix);
			shd->UniformMat4(std::string("ProjectionViewMatrix"), Renderer->ProjectionViewMatrix);

			i->GetSprite()->GetTexture()->Bind(0);
			shd->Uniformi(std::string("texture1"), 0);

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		Renderer->StowedActors.clear();
	}

	void Renderer2D::DrawTileMapHelper()
	{
		auto Renderer = Ermine::Renderer2D::Get();

		/*std::vector<std::vector<float>> VertexBuffer;
		std::vector<uint32_t> IndexBuffer;*/
		std::vector<Ermine::VertexArray> VaoArray;
		std::vector<std::unordered_map<std::filesystem::path, float>> TextureMappingCache;

		for (auto map : Renderer->StowedTileMaps)
		{
			auto Iter = Renderer->RenderPrimitiveCache.find(map->TileMapPath);
			if (Iter == Renderer->RenderPrimitiveCache.end())
			{
				//Not Found..
				for (auto Layer : map->GetAllLayers())
				{
					auto casket = Renderer->CreateVertexArrayForLayer(Layer, map);
					VaoArray.emplace_back(casket.first);
					TextureMappingCache.emplace_back(casket.second);
				}
				Ermine::TileMapRendererPrimitive RendererPrimitive;
				
				for (auto i : VaoArray)
					RendererPrimitive.SubmitLayerVao(i);

				for (auto i : TextureMappingCache)
					RendererPrimitive.SubmitLayerTextureToNumberCache(i);

				Renderer->RenderPrimitiveCache[map->TileMapPath] = RendererPrimitive;
			}

			auto DrawPrimitive = Renderer->RenderPrimitiveCache[map->TileMapPath];

			for (int i = 0; i < DrawPrimitive.GetNumberOfLayersStored(); i++)
			{
				Ermine::VertexArray* Vao = DrawPrimitive.GetVao(i);
				std::unordered_map<std::filesystem::path, float> TextureToNumberInVaoMapper = DrawPrimitive.GetTextureToNumberCache(i);

				static Ermine::Shader Shd(std::filesystem::path("Shader/Vertex/TileMapVertexShader.vert"), 
								   std::filesystem::path("Shader/Fragment/TileMapFragmentShader.frag"));

				Shd.Bind();
				Vao->Bind();

				std::vector<VertexAttribPointerSpecification> Spec = {
				{3,GL_FLOAT,false},
				{2,GL_FLOAT,false},
				{1,GL_FLOAT,false}
				};

				Vao->SetVertexAttribArray(Spec);

				static glm::mat4 ProjectionViewMatrix = glm::ortho<float>(0.0f, ((float)Ermine::GetScreenWidth()), ((float)Ermine::GetScreenHeight()), 0.0f, -5.0f, 5.0f);

				Shd.UniformMat4(std::string("ProjectionViewMatrix"), ProjectionViewMatrix); //Here See That The Projection MAtrix Is Akin To Screen Coordinates..

				auto TextureCacheGlobal = Ermine::GlobalTextureCache::Get();
				std::vector<float> TextureMappingUnits;
				TextureMappingUnits.resize(16);

				for (int i = 0; i < 16; i++)
					TextureMappingUnits[i] = 0.0f;

				for (auto i = TextureToNumberInVaoMapper.begin(); i != TextureToNumberInVaoMapper.end(); i++)
				{
					Ermine::Texture* Tex = TextureCacheGlobal->GetTextureFromFile(i->first);
					int BoundSlot = TextureCacheGlobal->Bind(Tex);

					TextureMappingUnits[(int)i->second] = BoundSlot;
				}
				
				Shd.UniformNf(std::string("Sampler2DArray"),TextureMappingUnits);

				GLCall(glDrawElements(GL_TRIANGLES, Vao->GetIndexBufferLength(), GL_UNSIGNED_INT, 0));
			}
		}
		Renderer->StowedTileMaps.clear();
	}

	std::pair<VertexArray, std::unordered_map<std::filesystem::path, float>> Renderer2D::CreateVertexArrayForLayer(Ermine::TileMap::Layer& layer,
																												   TileMap* tm)
	{
		int GridXToBeGenerated = layer.NumberOfTilesHorizontal;
		int GridYToBeGenerated = layer.NumberOfTilesVertical;

		float StepInX = Ermine::GetScreenHeight() / NumberOfGridsInXOnScreen;
		float StepInY = Ermine::GetScreenWidth() / NumberOfGridsInYOnScreen;

		//These Variables Are USed To Generate The VErtex Buffer..
		float CurrentPositionX = 0.0f;
		float CurrentPositionY = 0.0f;

		//This VAriable is used to generate the index buffer...
		int IndexCounter = 0;

		//This Number Is Used To Count The Texture Number...
		int TextureNumber = 0;

		std::vector<float> VertexBuffer;
		std::vector<uint32_t> IndexBuffer;

		std::unordered_map<std::filesystem::path, float> TextureToNumberMapper; //We Must Return This Too I Guess...

		int Count = 0;
		for (auto i : layer.LayerData)
		{
			if (i == 0)
			{
				//Are You Stupid All You Are Gonna GEt Is Diagnol...
				CurrentPositionX = CurrentPositionX + StepInX;
				Count++;
				if (Count >= layer.NumberOfTilesHorizontal)
				{
					Count = 0;
					CurrentPositionX = 0.0f;
					CurrentPositionY = CurrentPositionY + StepInY;
				}
				//CurrentPositionY = CurrentPositionY + StepInY;

				continue;
			}

			//Start GEtting The Index Buffer Ready..
			IndexBuffer.emplace_back(IndexCounter);
			IndexBuffer.emplace_back(IndexCounter + 1);
			IndexBuffer.emplace_back(IndexCounter + 3);

			IndexBuffer.emplace_back(IndexCounter + 1);
			IndexBuffer.emplace_back(IndexCounter + 2);
			IndexBuffer.emplace_back(IndexCounter + 3);

			IndexCounter = IndexCounter + 4;
			//Ended Getting The Index Buffer Ready..

			auto TextureNumberMapperIterator = TextureToNumberMapper.find(tm->GetSprite(i)->GetTexture()->GetFilePath());
			if (TextureNumberMapperIterator == TextureToNumberMapper.end())
			{
				//Not Found
				TextureToNumberMapper[tm->GetSprite(i)->GetTexture()->GetFilePath()] = TextureNumber++;
			}

			//Start Setting Up Top Right Vertex..
			VertexBuffer.emplace_back(CurrentPositionX + StepInX); //x
			VertexBuffer.emplace_back(CurrentPositionY + StepInY); //y
			VertexBuffer.emplace_back(0.0f); //z

			VertexBuffer.emplace_back(tm->GetSprite(i)->GetTopRightUV().x); //u
			VertexBuffer.emplace_back(tm->GetSprite(i)->GetTopRightUV().y); //v

			VertexBuffer.emplace_back(TextureToNumberMapper.find(tm->GetSprite(i)->GetTexture()->GetFilePath()).operator*().second); //Texture Number
			//Ended Setting Up Top Right Vertex..

			//Start Setting Up Bottom Right Vertex..
			VertexBuffer.emplace_back(CurrentPositionX + StepInX);
			VertexBuffer.emplace_back(CurrentPositionY);
			VertexBuffer.emplace_back(0.0f);

			VertexBuffer.emplace_back(tm->GetSprite(i)->GetTopRightUV().x);
			VertexBuffer.emplace_back(tm->GetSprite(i)->GetBottomLeftUV().y);

			VertexBuffer.emplace_back(TextureToNumberMapper.find(tm->GetSprite(i)->GetTexture()->GetFilePath()).operator*().second); //Texture Number
			//Ended Setting Up Bottom Right Vertex..

			//Start Setting Up Bottom Left Vertex..
			VertexBuffer.emplace_back(CurrentPositionX);
			VertexBuffer.emplace_back(CurrentPositionY);
			VertexBuffer.emplace_back(0.0f);

			VertexBuffer.emplace_back(tm->GetSprite(i)->GetBottomLeftUV().x);
			VertexBuffer.emplace_back(tm->GetSprite(i)->GetBottomLeftUV().y);

			VertexBuffer.emplace_back(TextureToNumberMapper.find(tm->GetSprite(i)->GetTexture()->GetFilePath()).operator*().second); //Texture Number
			//Ended Setting Up Bottom Left Vertex..

			//Start Setting Up Top Left Vertex..
			VertexBuffer.emplace_back(CurrentPositionX);
			VertexBuffer.emplace_back(CurrentPositionY + StepInY);
			VertexBuffer.emplace_back(0.0f);

			VertexBuffer.emplace_back(tm->GetSprite(i)->GetBottomLeftUV().x);
			VertexBuffer.emplace_back(tm->GetSprite(i)->GetTopRightUV().y);

			VertexBuffer.emplace_back(TextureToNumberMapper.find(tm->GetSprite(i)->GetTexture()->GetFilePath()).operator*().second); //Texture Number
			//Ended Setting Up Top Left Vertex..

			CurrentPositionX = CurrentPositionX + StepInX;

			Count++;
			if (Count >= layer.NumberOfTilesHorizontal)
			{
				Count = 0;
				CurrentPositionX = 0.0f;
				CurrentPositionY = CurrentPositionY + StepInY;
			}
			
		}
		return std::make_pair(Ermine::VertexArray(VertexBuffer, IndexBuffer),TextureToNumberMapper);
	}
}
