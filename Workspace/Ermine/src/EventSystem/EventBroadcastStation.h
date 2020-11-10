#pragma once
#include<memory>
#include<vector>
#include<string>

#include<thread>
#include<mutex>

#include "SubscriptionTicket.h"

#include "Event.h"

#include "EventTypes/TestConcreteEvent.h"
#include "EventTypes/KeyCallbackEvent.h"
#include "EventTypes/CharacterCallbackEvent.h"
#include "EventTypes/CursorPositionCallbackEvent.h"
#include "EventTypes/MouseButtonCallbackEvent.h"
#include "EventTypes/ScrollCallbackEvent.h"
#include "EventTypes/TileSelectedEvent.h"
#include "EventTypes/OnTickEvent.h"

//TODO Write a Macro Instead Of all those obnoxious helper functions...

namespace Ermine
{
	// Note - This is a Singleton Class Residing On a Seperate Thread Please See That You Take Care Of All Design Considerations...
	class EventBroadcastStation
	{
	private:
		EventBroadcastStation() = default; //Dont Think we Would Need a Custom Constructor Yet
		~EventBroadcastStation(); //A Destructor to avoid detecting memory leaks when the program closes besides its good practice right 

		static EventBroadcastStation* EventBroadcastStationPointer;
		static std::once_flag LazyInitializationFlag;
		static std::thread* StationThreadObject;
	public:
		static std::atomic<bool> StationDestructionOrdered; //This Flag Is Set Public Because There are some people who would like to know if god has been destroyed

	public:
		static std::mutex MainMutex;

	public:
		static EventBroadcastStation* GetStation(); //This is a static method and will allow the user to get a line with the station in question
		static void DestroyStation(); //This is a static method and will allow the user to Destroy the station in question

		void DispatchMessages(); //This Will Trigger Delievery of Messages //Note Lock Mutexes Appropriately...
		
		void CheckObjectsHealthAndDeleteLowHealth(); //Delete Objects Whose Health Is Low

		void QueueBroadcast(std::unique_ptr<Event> BroadcastPackage); //This Message Is Used To Send in a package for Broadcast
		
		Ermine::SubscriptionTicket  QueueSubscription(std::unique_ptr<EventSubscription> Subscription);
		void						DestroySubscription(Ermine::SubscriptionTicket SubscriptionTicket); //Note We Have No Way To Check Ownership Of Subscription For Now Be VERY CAREFUL...................................................
	private:
		//This Is The Storage Area We Will Store Everything Of Note Here...

		//All Different Events Are Stored In Different Containers So As To Parallilize Submitions.. Two Objects Wanting To Queue Two Different Types Of Events Need not wait ..   
		std::vector<ConcreteEvent> ConcreteEventsQueue; //These Are submitted events..
		std::vector<KeyCallbackEvent> KeyCallbackEventsQueue;
		std::vector<CharacterCallbackEvent>CharacterCallbackEventsQueue;
		std::vector<CursorPositionCallbackEvent>CursorPositionCallbackEventsQueue;
		std::vector<MouseButtonCallbackEvent>MouseButtonCallbackEventsQueue;
		std::vector<ScrollCallbackEvent>ScrollCallbackEventsQueue;
		std::vector<TileSelectedEvent> TileSelectedCallbackEventsQueue;
		std::vector<OnTickEvent> OnTickCallbackEventsQueue;
		//Donot Forget to add destructors for these containers inside the destructor..

	private:
		//We Will Store The Subscriptions Here...
		/*std::vector<ConcreteEventSubscription> ConcreteEventSubscriptions;
		std::vector<KeyCallbackEventSubscription> KeyCallbackEventsSubscriptions;
		std::vector<CharacterCallbackEventSubscription> CharacterCallbackEventSubscriptions; 
		std::vector<CursorPositionCallbackEventSubscription> CursorPositionCallbackEventSubscriptions;
		std::vector<MouseButtonCallbackEventSubscription> MouseButtonCallbackEventSubscriptions;
		std::vector<ScrollCallbackEventSubscription> ScrollCallbackEventSubscriptions;
		std::vector<TileSelectedEventSubscription> TileSelectedCallbackEventSubscriptions;
		std::vector<OnTickEventSubscription> OnTickCallbackEventSubscriptions;*/

		//More Modern Approach X>
		std::unordered_map<int,ConcreteEventSubscription> ConcreteEventSubscriptions;
		std::unordered_map<int,KeyCallbackEventSubscription> KeyCallbackEventsSubscriptions;
		std::unordered_map<int,CharacterCallbackEventSubscription> CharacterCallbackEventSubscriptions;
		std::unordered_map<int,CursorPositionCallbackEventSubscription> CursorPositionCallbackEventSubscriptions;
		std::unordered_map<int,MouseButtonCallbackEventSubscription> MouseButtonCallbackEventSubscriptions;
		std::unordered_map<int,ScrollCallbackEventSubscription> ScrollCallbackEventSubscriptions;
		std::unordered_map<int,TileSelectedEventSubscription> TileSelectedCallbackEventSubscriptions;
		std::unordered_map<int,OnTickEventSubscription> OnTickCallbackEventSubscriptions;

		//Donot Forget to add destructors for these containers inside the destructor..

	private:
		//Helper Functions For Dispatching Stuff To Right Destinations---
		void DispatchConcreteMessages();
		void DispatchKeyCallbackMessages();
		void DispatchCharacterCallbackMessages();
		void DispatchCursorPositionCallbackMessages();
		void DispatchMouseButtonCallbackMessages();
		void DispatchScrollCallbackMessages();
		void DispatchTileSelectedCallbackMessages();
		void DispatchOnTickCallbackMessages();

		//This Method Gets A Subscription Ticket If A Slot Is Available.. The Bool is also Set True Remember.. 
		int GetOpenSubscriptionTicket();
		void CloseSubscriptionTicket(int SubscriptionTicket);

	private:

		//This Container
		static std::vector<bool> SubscriptionTicketsInUse;

	};

}