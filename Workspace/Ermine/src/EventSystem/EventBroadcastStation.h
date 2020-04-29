#pragma once
#include<memory>
#include<vector>
#include<string>

#include<thread>
#include<mutex>

#include "Event.h"
#include "EventTypes/TestConcreteEvent.h"

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

	private:
		//We Will Store The Subscriptions Here...
		std::vector<ConcreteEventSubscription> ConcreteEventSubscriptions;
	};

}