#pragma once
#include<iostream>

#include<vector>
#include<unordered_map>

#include<string>

#include<mutex>
#include<atomic>

#include<filesystem>
#include<functional>

#include "Log.h"
#include "EventSystem/EnumEventType.h"

#include "ObjectStatus.h"
#include "InteriorComponents/GeneratedObject.h"

/*
*	Note - Remember The Paradigm Well.. No Object Must Ever Be Owned By any Thread.. Always Generate Them Using Factory Methods Which Give To You Shared Pointers
*/

namespace Ermine
{
	class Event;

	class SubscriptionTicket;

	class Object
	{
	public:
#pragma region Constructors
		//Try Not To Use This Constructor As It Leads To The Object Being Name DefaultObject..
		Object();

		Object(std::string Name);

		Object(const Object& rhs);
		Object& operator=(const Object& rhs);

		Object(Object&& rhs);
		Object& operator=(Object&& rhs);

		//Virtual Destructor As Object Is Most Likely Inherited..
		virtual ~Object(); 
#pragma endregion

		//Use This Function To Get A Handle To The Generated Object.. DONOT USE THIS IF NOT ABSOLUTELY NECESSARY
		std::shared_ptr<Ermine::GeneratedObject> GetSharedPtrToObject() { return GeneratedObjPtr; };

#pragma region ObjectHealthQuery
		Ermine::ObjectStatus GetObjectHealth();

		void SetObjectHealth(Ermine::ObjectStatus Status);
		void MarkObjectForDeletion(); 
		void RestoreObjectHealth(); 
#pragma endregion 

#pragma region PushToFunction
		//The Assigned Function Is Stored In The Object.. Is Not Copied.. And The Events Recieved By The Object Is Directly Pushed Into The Said Function Without Any Processing..
		void AssignPushEventsToFunction(std::function<void(Ermine::Event*)> PushToFunction);
		void DestroyPushEventsToFunction();

		bool QueryPushEventsToFunctionFlag();
		std::function<void(Ermine::Event*)> GetPushToFunction();
#pragma endregion PushToFunction

#pragma region InterfaceToInteractWithGeneratedObject
		bool IsRecievingConcreteEvents();			 
		bool IsRecievingKeyCallbackEvents();		 
		bool IsRecievingCharacterKeyCallbackEvents();
		bool IsRecievingCursorPositionUpdateEvents();
		bool IsRecievingMouseButtonCallbackEvents ();
		bool IsRecievingScrollUpdateEvents();		 
		bool IsRecievingTileSelectedCallbackEvents();
		bool IsRecievingOnRenderTickEvents();
		bool IsRecievingOnUpdateTickEvents();
		bool IsRecievingOnBeginEvents();			 

		//Recieve All Flags As A Vector Of Bool Instead Of Specified Singular Bool
		std::vector<bool> AllFlagsOfRecievingEvents();

		std::unique_lock<std::recursive_mutex> GetObjectMutex() { return std::move(GeneratedObjPtr->GetObjectMutex()); }

		std::string GetUniqueIdentifier();

		void RecieveEvents(bool Flag, Ermine::EventType TypeOfEventToRecieve);// {  }
#pragma endregion

#pragma region EventReciever
		virtual void ConcreteEventRecieved(std::string MessageBuffer) { DefaultEventHandler(); };
		virtual void KeyCallbackEventRecieved(int key, int scancode, int action, int mods) { DefaultEventHandler(); };
		virtual void CharacterkeyCallbackEventRecieved(int Codepoint) { DefaultEventHandler(); };
		virtual void CursorPositionUpdateEventRecieved(int xpos, int ypos) { DefaultEventHandler(); };
		virtual void MouseButtonCallbackEventRecieved(int button, int action, int mods) { DefaultEventHandler(); };
		virtual void ScrollPositionUpdateEventRecieved(double xoffset, double yoffset) { DefaultEventHandler(); };
		
		virtual void OnRenderTickEventRecieved(float DeltaTime) { DefaultEventHandler(); };
		virtual void OnUpdateTickEventRecieved() { DefaultEventHandler(); };

		virtual void OnBeginEvent(std::shared_ptr<void> Packet) { DefaultEventHandler(); };

		virtual void TileSelectedUpdateEventRecieved(std::filesystem::path TilesetPath, int index) { DefaultEventHandler(); };

	private:
		void DefaultEventHandler();
#pragma endregion

	private:
		//The Object Name Is Not Required To Be Unique.. Although Try To Keep Ut Unique..
		std::string Object_Name = "DefaultObject";

		//This Is The Object Health.. SInce Nobody Can Explicitly Delete An Object.. Anybody Can Request Its Initiation Of Deletion
		ObjectStatus ObjectHealth = Ermine::ObjectStatus::StatusOk;

		//Hold A Pointer To A Generated Object
		std::shared_ptr<Ermine::GeneratedObject> GeneratedObjPtr;

		//A Special Function Pointer Can Be Assigned To Get The Event Without Usual Processing..
		std::function<void(Ermine::Event*)> PushToFunction = nullptr;
		bool PushToFunc = false;
	};

	
}