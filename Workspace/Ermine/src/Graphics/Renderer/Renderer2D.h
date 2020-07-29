#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<mutex>

#include "Core.h"
#include "glm.hpp"
#include "LayerSystem/LayerStack.h"
#include "EngineResourceHandlers/GlobalTextureCache.h"

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
		//static void SubmitLayerStack(LayerStack& layerstack); //This Has Been Commented Out So As To Avoid Ambiguity.. Why Call A Method Which HAs Not Yet Been Implemented
		
		//This is a smooth function clears layer stack in the renderer and places this stack in its place..
		static void ReplaceLayerStackWithStack(LayerStack layerstack);

		static void EndScene();

	public:

	protected:

	protected:

	private:
		void DrawingRoutine();
		
	private:
		static std::once_flag InitializationFlag;
		static Renderer2D* GlobalRenderer2DObj;

		glm::mat4 CameraMatrix;
		glm::mat4 ProjectionMatrix;
		glm::mat4 ProjectionViewMatrix;

		bool SceneBegin;

		LayerStack RendererLayerStack;
	};
}