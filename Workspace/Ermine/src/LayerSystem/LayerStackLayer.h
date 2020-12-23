#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<memory>

#include "EventSystem/Event.h"

#include "Graphics/Renderer/RenderableComponents/Renderable2D.h"
#include "2DPrimitives/ActorFamily/Actor2D.h"
#include "2DPrimitives/ActorFamily/Actor2DShape.h"

#include "2DPrimitives/ActorFamily/PhysicsActor2D.h"
#include "Graphics/Renderer/RenderableComponents/RenderablePhysicsComponent2D.h"

#include "2DPrimitives/TileMapLayerRenderable.h"
#include "2DPrimitives/Label.h"

#include "Physics/Physics.h"

namespace Ermine
{
	class LayerStack;

	class LayerStackLayer
	{
	public:
		LayerStackLayer(std::string Name);
		virtual ~LayerStackLayer();

	public:
		//The Copy Stuff Is Out.. As All Renderables Are Packed Into Unique Pointers
		LayerStackLayer(const LayerStackLayer& rhs) = default;
		LayerStackLayer& operator=(const LayerStackLayer& rhs) = default;

		//Use This In The Place Of Copy If Required..
		LayerStackLayer(LayerStackLayer&& rhs) = default;
		LayerStackLayer& operator=(LayerStackLayer&& rhs) = default;

		std::string GetName() {	return LayerName; };
		void SetName(std::string Name) { LayerName = Name; };

	public:
		//Use This Function To Construct A Layer.. (This Function Is Called Only Once)
		virtual void ConstructLayer() {};

		//This Function Is Called Every UpdateCycle..
		virtual void Update(float Dt) {};

		//This Function Is Called Every Render Cycle..
		virtual void Render(float Dt) {};

	protected:
#pragma region EventHandlerFunctions
		virtual bool ConcreteEventRecieved(std::string MessageBuffer) { return false; };
		virtual bool KeyCallbackEventRecieved(int key, int scancode, int action, int mods) { return false; };
		virtual bool CharacterkeyCallbackEventRecieved(int Codepoint) { return false; };
		virtual bool CursorPositionUpdateEventRecieved(int xpos, int ypos) { return false; };
		virtual bool MouseButtonCallbackEventRecieved(int button, int action, int mods) { return false; };
		virtual bool ScrollPositionUpdateEventRecieved(double xoffset, double yoffset) { return false; };
		//Tick,BeginEvent,TileSelectedUpdateEvent Are Not Supported As Of Now.. 
#pragma endregion
	protected:

	private:
		//Returns True If Handle Is Successfull.. If True Is Returned Maybe Dont Propogate Further.. Of Course Thats Upto LayerStack 
		bool HandleEvents(Ermine::Event* EventPointer);

	private:
		std::string LayerName = "NoName";

		friend class LayerStack;
	};
}