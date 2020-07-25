#pragma once
#include<iostream>
#include<vector>
#include<string>

#include<mutex>

#include "Core.h"

#include "EngineResourceHandlers/GlobalTextureCache.h" //This Is Now Primarirly Used For The HAsh Function Of The Unordered Map..

#include "glm.hpp"
#include "2DPrimitives/Renderable2D.h"
#include "2DPrimitives/Actor2D.h"
#include "2DPrimitives/TileMap.h"
#include "2DPrimitives/TileMapLayerRenderable.h"

#include "Graphics/Renderer/MaterialSystem/Shader.h"
#include "Graphics/Renderer/MaterialSystem/Texture.h"

#include "RendererPrimitives/VertexArray.h"
//#include "RendererPrimitives/RendererPrimitives2D/TileMapRendererPrimitive.h"

#include "LayerSystem/LayerStack.h"

namespace Ermine
{
	class Renderer2D
	{
	public:
		Renderer2D();
		~Renderer2D(); //Renderer2D Destructor..

	public:
		static Renderer2D* Get();

		static void BeginScene(glm::mat4 CameraMAtrix,glm::mat4 ProjectionMatrix);

		//This Is THe Submition Method, It Appends The LAyer To The front Of The Screen
		static void SubmitLayer(LayerStackLayer layer);

		//This Is Alternative Submission Method It Places The Layer Wherever You want..
		static void SubmitLayer(LayerStackLayer layer, int index);

		//This Will Be Implemented Properly Another Day When The Event System Is Properly Fixed.. For Now It just copies Over The Layers into the layer stack held by the renderer
		static void SubmitLayerStack(LayerStack& layerstack);
		
		//This is a smooth function clears layer stack in the renderer and places this stack in its place..
		static void ReplaceLayerStackWithStack(LayerStack layerstack);

		//static void DrawActor2D(Actor2D* Act);
		//static void DrawTileMap(TileMap* Tm);

		static void EndScene();

		//static void SetNumberOfGridsOnScreen(glm::vec<2, int> NumberOnXAndY);
		
		//void ClearStowedActors();

	public:
		//Shader Cache
		//Ermine::Shader* Actor2DShader;
		//Ermine::Shader* TileMapShader;

	protected:

	protected:

	private:
		void DrawingHelper();
		//static void DrawActorsHelper();
		//void DrawTileMapHelper();

		//std::pair<VertexArray, std::unordered_map<std::filesystem::path, float>> CreateVertexArrayForLayer(Ermine::TileMap::Layer& layer,
			//																							   TileMap* tm);

	private:
		static std::once_flag InitializationFlag;
		static Renderer2D* GlobalRenderer2DObj;

		glm::mat4 CameraMatrix;
		glm::mat4 ProjectionMatrix;
		glm::mat4 ProjectionViewMatrix;

		bool SceneBegin;

		LayerStack RendererLayerStack;
		//These Buffers Are Irrelevant
		/*std::vector<Actor2D*> StowedActors;
		std::vector<TileMap*> StowedTileMaps;*/

		/*//Variables Exclusive To TileMap Rendering
		int NumberOfGridsInXOnScreen = 10;
		int NumberOfGridsInYOnScreen = 10;*/
	};
}