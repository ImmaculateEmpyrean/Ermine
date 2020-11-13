#pragma once
#include<iostream>

#include<vector>
#include<unordered_map>

#include<string>

#include<mutex>
#include<atomic>

#include<filesystem>
#include<functional>

#include "EventSystem/EnumEventType.h"

/*
*	Note - Remember The Paradigm Well.. No Object Must Ever Be Owned By any Thread.. Always Generate Them Using Factory Methods Which Give To You Shared Pointers
*/

namespace Ermine
{
	class Event;

	class SubscriptionTicket;

	enum class ObjectStatus: unsigned int
	{
		StatusMarkedForDeletion=0,
		StatusOk=1
	};

	class Object
	{
	private:
		//Object Can Be Constructed Without Much Prior Construction For Now.. It Just Cant Be Constructed Defaultly
		Object();

	public:
		//Start Since Object Contains A Mutex Lets Go With Defining Copy And Move Operations..//
		Object(Object& rhs);
		Object& operator=(Object& rhs);

		Object(Object&& rhs);
		Object& operator=(Object&& rhs);
		//Ended Since Object Contains A Mutex Lets Go With Defining Copy And Move Operations..//
	public:
		std::string GetUniqueIdentifier() { return UniqueIdentifier; }
		
		//Use This Function To CleanUp Delete Ready Objects
		static void FlushUnusedObjects();

		//Only This Method Must Give Out An Object.. Use It To Get A Shared Pointer To An Object You Know Or Generate A New Object..DONOT EVER CREATE SHARED_POINTERS TO OBJECTS YOURSELF!!!!!!
		static std::shared_ptr<Ermine::Object> GenerateObject(std::string UniqueIdentifier = "");

		Ermine::ObjectStatus GetObjectHealth() { return ObjectHealth; }

		void SetObjectHealth(Ermine::ObjectStatus Status) { ObjectHealth = Status; }
		void MarkObjectForDeletion() { ObjectHealth = Ermine::ObjectStatus::StatusMarkedForDeletion; };
		void RestoreObjectHealth() { { ObjectHealth = Ermine::ObjectStatus::StatusOk; }; }

		std::unique_lock<std::recursive_mutex> GetObjectMutex() { return std::move(std::unique_lock<std::recursive_mutex>(ObjectMutex)); };

		//Start Event Subscriptions//
		void EventReciever(Ermine::Event* Eve);
		
		//Use This Method To Set Which Events Must Be Recieved By The Object..
		void RecieveEvents(bool Recieve, Ermine::EventType Type);

		bool IsRecievingConcreteEvents()			 { return FlagsOfRecievingEvents[(unsigned int)Ermine::EventType::ConcreteEvent]; }
		bool IsRecievingKeyCallbackEvents()			 { return FlagsOfRecievingEvents[(unsigned int)Ermine::EventType::KeyCallbackEvent]; }
		bool IsRecievingCharacterKeyCallbackEvents() { return FlagsOfRecievingEvents[(unsigned int)Ermine::EventType::CharacterCallbackEvent]; }
		bool IsRecievingCursorPositionUpdateEvents() { return FlagsOfRecievingEvents[(unsigned int)Ermine::EventType::CursorPositionCallbackEvent]; };
		bool IsRecievingMouseButtonCallbackEvents()  { return FlagsOfRecievingEvents[(unsigned int)Ermine::EventType::MouseButtonCallbackEvent]; };
		bool IsRecievingScrollUpdateEvents()		 { return FlagsOfRecievingEvents[(unsigned int)Ermine::EventType::ScrollCallbackEvent];}
		bool IsRecievingTileSelectedCallbackEvents() { return FlagsOfRecievingEvents[(unsigned int)Ermine::EventType::TileSelectedEvent];};
		bool IsRecievingOnTickEvents()				 { return FlagsOfRecievingEvents[(unsigned int)Ermine::EventType::OnTickEvent];};

		virtual void ConcreteEventRecieved			  (std::string MessageBuffer)					   { DefaultEventHandler(); };
		virtual void KeyCallbackEventRecieved		  (int key, int scancode, int action, int mods)	   { DefaultEventHandler(); };
		virtual void CharacterkeyCallbackEventRecieved(int Codepoint)								   { DefaultEventHandler(); };
		virtual void CursorPositionUpdateEventRecieved(int xpos, int ypos)							   { DefaultEventHandler(); };
		virtual void MouseButtonCallbackEventRecieved (int button, int action, int mods)			   { DefaultEventHandler(); };
		virtual void ScrollPositionUpdateEventRecieved(double xoffset, double yoffset)				   { DefaultEventHandler(); };
		virtual void OnTickEventRecieved			  (float DeltaTime)								   { DefaultEventHandler(); };

		virtual void TileSelectedUpdateEventRecieved  (std::filesystem::path TilesetPath, int index)   { DefaultEventHandler(); };

	public:

	protected:

	protected:

	private:
		virtual void Dummy() {}

		//This Function Just Tries Printing DebugInfo If Something Goes Wrong..
		void DefaultEventHandler() {/* Fill This Default Handler */}; 

	private:
		static std::atomic<long> Counter;
		std::string UniqueIdentifier;

		ObjectStatus ObjectHealth = Ermine::ObjectStatus::StatusOk;

		//Flags To Check And Tickets To Store If A Certain Event Is Executing..
		std::function<void(std::string)> ConcreteEventFunctionPointer = nullptr;
		std::function<void(int, int, int, int)> KeyCallBackEventFunctionPointer = nullptr;
		std::function<void(int)> CharacterCallbackEventFunctionPointer = nullptr;
		std::function<void(int, int)> CursorPositionUpdateEventFunctionPointer = nullptr;
		std::function<void(int, int, int)> MouseButtonCallbackFunctionPointer = nullptr;
		std::function<void(double, double)>ScrollPositionUpdateFunctionPointer = nullptr;
		std::function<void(float)> OnTickEventFunctionPointer = nullptr;
		std::function<void(std::filesystem::path, int)> TileSelectedUpdateFunctionPointer = nullptr;

		std::vector<bool> FlagsOfRecievingEvents;
		std::vector<Ermine::SubscriptionTicket*> TicketsHeldByTheObject;

		std::atomic<bool> ObjectReadyToRecieveEvents = false;

		std::recursive_mutex ObjectMutex;

		static std::unordered_map<std::string, std::shared_ptr<Ermine::Object>> FactoryHashTable;
		static std::recursive_mutex FactoryHashTableMutex;
	};
}