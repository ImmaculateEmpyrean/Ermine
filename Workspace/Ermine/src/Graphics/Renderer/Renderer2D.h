#pragma once
#include<iostream>
#include<vector>
#include<string>

#include<mutex>

#include "Core.h"

#include "EngineResourceHandlers/GlobalTextureCache.h" //This Is Now Primarirly Used For The HAsh Function Of The Unordered Map..

#include "glm.hpp"
#include "2DPrimitives/Actor2D.h"
#include "2DPrimitives/TileMap.h"

#include "RendererPrimitives/VertexArray.h"
#include "RendererPrimitives/RendererPrimitives2D/TileMapRendererPrimitive.h"

namespace Ermine
{
	class Renderer2D
	{
	public:
		Renderer2D() = default;
		~Renderer2D(); //Renderer2D Destructor..

	public:
		static Renderer2D* Get();

		static void BeginScene(glm::mat4 CameraMAtrix,glm::mat4 ProjectionMatrix);

		static void DrawActor2D(Actor2D* Act);
		static void DrawTileMap(TileMap* Tm);

		static void EndScene();

		static void SetNumberOfGridsOnScreen(glm::vec<2, int> NumberOnXAndY);
		
		void ClearStowedActors();

	public:

	protected:

	protected:

	private:
		static void DrawActorsHelper();
		static void DrawTileMapHelper();

		std::pair<VertexArray, std::unordered_map<std::filesystem::path, float>> CreateVertexArrayForLayer(Ermine::TileMap::Layer& layer,
																										   TileMap* tm);

	private:
		static std::once_flag InitializationFlag;
		static Renderer2D* GlobalRenderer2DObj;

		glm::mat4 CameraMatrix;
		glm::mat4 ProjectionMatrix;
		glm::mat4 ProjectionViewMatrix;

		bool SceneBegin;

		std::vector<Actor2D*> StowedActors;
		std::vector<TileMap*> StowedTileMaps;

		//Variables Exclusive To TileMap Rendering
		int NumberOfGridsInXOnScreen = 10;
		int NumberOfGridsInYOnScreen = 10;
		
		std::unordered_map<std::filesystem::path, Ermine::TileMapRendererPrimitive> RenderPrimitiveCache;
	};
}