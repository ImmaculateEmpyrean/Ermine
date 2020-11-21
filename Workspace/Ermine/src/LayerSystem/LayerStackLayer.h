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
	//Forward Declaration For The Purposes Of Being a friend...
	class Renderer2D;
	class LayerStack;

	class LayerStackLayer
	{
	public:
		LayerStackLayer(std::string Name);
		~LayerStackLayer();

	public:
		//The Copy Stuff Is Out.. As All Renderables Are Packed Into Unique Pointers
		LayerStackLayer(const LayerStackLayer& rhs) = delete;
		LayerStackLayer& operator=(const LayerStackLayer& rhs) = delete;

		//Use This In The Place Of Copy If Required..
		LayerStackLayer(LayerStackLayer&& rhs);
		LayerStackLayer& operator=(LayerStackLayer&& rhs);

		//This Function Generates A Renderable For The Actor In Memory.. Submit The Actor Twice For No Effect Whatsover Except Double Computation For Now.. In Future Maybe We Will Leverege This Archietecture.. 
		void SubmitActor(std::shared_ptr<Actor2DBase> Ptr);

		//The Function Does Nothing For Now.. Will Not Be The Case By The Time.. HillClimbRacing Comes v1.0 Is Released..
		void AddLabel(std::string Text, glm::vec3 Color,glm::vec3 PositionInScreenCoordinates,std::string FontName);

		//Clear All Submitted Renderables
		void Clear();

		std::string GetName();

	public:

	protected:
#pragma region EventHandlerFunctions
		virtual bool ConcreteEventRecieved(std::string MessageBuffer) { DefaultEventHandler(); return false; };
		virtual bool KeyCallbackEventRecieved(int key, int scancode, int action, int mods) { DefaultEventHandler(); return false; };
		virtual bool CharacterkeyCallbackEventRecieved(int Codepoint) { DefaultEventHandler(); return false; };
		virtual bool CursorPositionUpdateEventRecieved(int xpos, int ypos) { DefaultEventHandler(); return false; };
		virtual bool MouseButtonCallbackEventRecieved(int button, int action, int mods) { DefaultEventHandler(); return false; };
		virtual bool ScrollPositionUpdateEventRecieved(double xoffset, double yoffset) { DefaultEventHandler(); return false; };
		//Tick,BeginEvent,TileSelectedUpdateEvent Are Not Supported As Of Now.. 
#pragma endregion
	protected:

	private:
		//Returns True If Handle Is Successfull.. If True Is Returned Maybe Dont Propogate Further.. Of Course Thats Upto LayerStack 
		bool HandleEvents(Ermine::Event* EventPointer);

		//This Function Just Generates A Log Message On The STD_OUT LOg.. Saying You Wasted A Call.. Maybe Useful For Debugging Who Knows?
		void DefaultEventHandler();

	private:
		std::string LayerName;
		std::vector<std::unique_ptr<Ermine::Renderable2D>> Renderables; //Made Them Unique Pointers As I Dont Expect Them To Have References Anywhere..

		friend class Ermine::Renderer2D;
		friend class Ermine::LayerStack;
	};
}