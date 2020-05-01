#pragma once
#include<memory>
#include<vector>
#include<string>

#include<thread>
#include<mutex>

#include "Event.h"

#include "EventTypes/TestConcreteEvent.h"
#include "EventTypes/KeyCallbackEvent.h"
#include "EventTypes/CharacterCallbackEvent.h"
#include "EventTypes/CursorPositionCallbackEvent.h"
#include "EventTypes/MouseButtonCallbackEvent.h"
#include "EventTypes/ScrollCallbackEvent.h"

//TODO Write a Macro Instead Of all those obnoxious helper functions...

namespace Ermine
{
	// Note - This is a Singleton Class Residing On a Seperate Thread Please See That You Take Care Of All Design Considerations...
	class EventBroadcastStation
	{
	private:
		EventBroadcastStation() = default; //Dont Think we Would Need a Custom Constructor Yet
		static EventBroadcastStation* EventBroadcastStationPointer;
		static std::once_flag LazyInitializationFlag;

	private:
		//This Section Contains an assortments of locks
		//static std::mutex GetStationLock; 
	public:
		static std::mutex MainMutex;
		//std::mutex QueueBroadcastLock;
		//std::mutex QueueSubscriptionLock;

	public:
		static EventBroadcastStation* GetStation(); //This is a static method and will allow the user to get a line with the station in question
		void DispatchMessages(); //This Will Trigger Delievery of Messages //Note Lock Mutexes Appropriately...
		
		void QueueBroadcast(std::unique_ptr<Event> BroadcastPackage); //This Message Is Used To Send in a package for Broadcast
		void QueueSubscription(std::unique_ptr<EventSubscription> Subscription);

	private:
		//This Is The Storage Area We Will Store Everything Of Note Here...
		std::vector<ConcreteEvent> ConcreteEventsQueue; //These Are submitted events..
		std::vector<KeyCallbackEvent> KeyCallbackEventsQueue;
		std::vector<CharacterCallbackEvent>CharacterCallbackEventsQueue;
		std::vector<CursorPositionCallbackEvent>CursorPositionCallbackEventsQueue;
		std::vector<MouseButtonCallbackEvent>MouseButtonCallbackEventsQueue;
		std::vector<ScrollCallbackEvent>ScrollCallbackEventsQueue;

	private:
		//We Will Store The Subscriptions Here...
		std::vector<ConcreteEventSubscription> ConcreteEventSubscriptions;
		std::vector<KeyCallbackEventSubscription> KeyCallbackEventsSubscriptions;
		std::vector<CharacterCallbackEventSubscription> CharacterCallbackEventSubscriptions;
		std::vector<CursorPositionCallbackEventSubscription> CursorPositionCallbackEventSubscriptions;
		std::vector<MouseButtonCallbackEventSubscription> MouseButtonCallbackEventSubscriptions;
		std::vector<ScrollCallbackEventSubscription> ScrollCallbackEventSubscriptions;

	private:
		//Helper Functions For Dispatching Stuff To Right Destinations---
		void DispatchConcreteMessages();
		void DispatchKeyCallbackMessages();
		void DispatchCharacterCallbackMessages();
		void DispatchCursorPositionCallbackMessages();
		void DispatchMouseButtonCallbackMessages();
		void DispatchScrollCallbackMessages();
		

	};

}