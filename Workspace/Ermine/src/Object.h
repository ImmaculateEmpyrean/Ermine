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
		Object();

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
		Ermine::ObjectStatus GetObjectHealth() { return ObjectHealth; }

		void SetObjectHealth(Ermine::ObjectStatus Status) { ObjectHealth = Status; }
		void MarkObjectForDeletion() { ObjectHealth = Ermine::ObjectStatus::StatusMarkedForDeletion; };
		void RestoreObjectHealth() { { ObjectHealth = Ermine::ObjectStatus::StatusOk; }; }
#pragma endregion 

#pragma region InterfaceToInteractWithGeneratedObject
		bool IsRecievingConcreteEvents()			 { return GeneratedObjPtr->IsRecievingConcreteEvents(); }
		bool IsRecievingKeyCallbackEvents()			 { return GeneratedObjPtr->IsRecievingKeyCallbackEvents(); }
		bool IsRecievingCharacterKeyCallbackEvents() { return GeneratedObjPtr->IsRecievingCharacterKeyCallbackEvents(); }
		bool IsRecievingCursorPositionUpdateEvents() { return GeneratedObjPtr->IsRecievingCursorPositionUpdateEvents(); };
		bool IsRecievingMouseButtonCallbackEvents () { return GeneratedObjPtr->IsRecievingMouseButtonCallbackEvents(); };
		bool IsRecievingScrollUpdateEvents()		 { return GeneratedObjPtr->IsRecievingScrollUpdateEvents(); }
		bool IsRecievingTileSelectedCallbackEvents() { return GeneratedObjPtr->IsRecievingTileSelectedCallbackEvents(); };
		bool IsRecievingOnTickEvents()				 { return GeneratedObjPtr->IsRecievingOnTickEvents(); };
		bool IsRecievingOnBeginEvents() { return GeneratedObjPtr->IsRecievingOnBeginEvents(); };

		//Recieve All Flags As A Vector Of Bool Instead Of Specified Singular Bool
		std::vector<bool> AllFlagsOfRecievingEvents() { return GeneratedObjPtr->AllFlagsOfRecievingEvents(); }

		std::unique_lock<std::recursive_mutex> GetObjectMutex() { return std::move(GeneratedObjPtr->GetObjectMutex()); }

		std::string GetUniqueIdentifier() { return GeneratedObjPtr->GetUniqueIdentifier(); }

		void RecieveEvents(bool Flag, Ermine::EventType TypeOfEventToRecieve) { GeneratedObjPtr->RecieveEvents(Flag, TypeOfEventToRecieve); }
#pragma endregion

#pragma region EventReciever
		virtual void ConcreteEventRecieved(std::string MessageBuffer) { DefaultEventHandler(); };
		virtual void KeyCallbackEventRecieved(int key, int scancode, int action, int mods) { DefaultEventHandler(); };
		virtual void CharacterkeyCallbackEventRecieved(int Codepoint) { DefaultEventHandler(); };
		virtual void CursorPositionUpdateEventRecieved(int xpos, int ypos) { DefaultEventHandler(); };
		virtual void MouseButtonCallbackEventRecieved(int button, int action, int mods) { DefaultEventHandler(); };
		virtual void ScrollPositionUpdateEventRecieved(double xoffset, double yoffset) { DefaultEventHandler(); };
		
		virtual void OnTickEventRecieved(float DeltaTime) { DefaultEventHandler(); };
		virtual void OnBeginEvent(std::shared_ptr<void> Packet) { DefaultEventHandler(); };

		virtual void TileSelectedUpdateEventRecieved(std::filesystem::path TilesetPath, int index) { DefaultEventHandler(); };

	private:
		void DefaultEventHandler();
#pragma endregion

	private:
		//This Is The Object Health.. SInce Nobody Can Explicitly Delete An Object.. Anybody Can Request Its Initiation Of Deletion
		ObjectStatus ObjectHealth = Ermine::ObjectStatus::StatusOk;

		//Hold A Pointer To A Generated Object
		std::shared_ptr<Ermine::GeneratedObject> GeneratedObjPtr;
	};

	
}