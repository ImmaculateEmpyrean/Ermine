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

		//Start Functions To Interact With The Flag Start Physics Debugger//
		static void TurnOnPhysicsDebugger();
		static void TurnOffPhysicsDebugger();
		//Ended Functions To Interact With The Flag Start Physics Debugger//

		//Start Use These Functions To Interact With The Buffer Which Determines Which PhysicsComponent2D Objects Have Their Shape outlines Drawn..//

		//This Function Does Not Own The Memory Allocated To The Pointer Donot Think Of Freeing The Memory At Any Point
		static void SubmitPhysicsComponent2D(PhysicsComponent2D* PhyComp);

		//This Function Removes A PhysicsComponent2D If Its Being Tracked And Its Shapes Are Drawn In RealTime..
		static void RemovePhysicsComponent2D(PhysicsComponent2D* PhyComp);

		//This Function Is Used To Clear Out The Buffer Of The Components Which Have To Be Drawn Not Free Their Memory..
		static void ClearPhysicsTrackingBuffer();

		//Ended Use These Functions To Interact With The Buffer Which Determines Which PhysicsComponent2D Objects Have Their Shape outlines Drawn..//

	public:

	protected:

	protected:

	private:
		void DrawingRoutine();
		void PhysicsDebuggerDrawingRoutine();

	private:
		//Tools To Make This Class a Singleton..
		static std::once_flag InitializationFlag;
		static Renderer2D* GlobalRenderer2DObj;

		//Various Matrices To Transform The World To Better Suit What We Are Drawing..
		glm::mat4 CameraMatrix;
		glm::mat4 ProjectionMatrix;
		glm::mat4 ProjectionViewMatrix;

		//Start Flags//
		bool SceneBegin;

		//This Flag Is Used To Turn On The Physics Debugger Which Is Responsible For Drawing Outlines Of The Shapes IN The Box2D World.. 
		bool UsePhysicsDebugger = false;

		//Ended Flags//

		//This Is The Buffer Which Has To Be Drawn This Frame..
		LayerStack RendererLayerStack;

		//This Buffer Stores All The Physics Components We Are Required to Draw The Outline Off..
		std::vector<PhysicsComponent2D*> PhysicsComponentsBuffer;

	};
}