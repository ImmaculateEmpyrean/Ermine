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
#include "EventTypes/OnBeginEvent.h"

//TODO Write a Macro Instead Of all those obnoxious helper functions...

namespace Ermine
{
	//Forward Declaration Of The Class Object So That Station Can Accept Subscriptions From It..
	class Object;

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
		

	public:
		static EventBroadcastStation* GetStation(); //This is a static method and will allow the user to get a line with the station in question
		static void DestroyStation(); //This is a static method and will allow the user to Destroy the station in question

		void DispatchMessagesSuperior(); //This Will Trigger Delievery of Messages //Note Lock Mutexes Appropriately...
				
		void QueueBroadcast(std::unique_ptr<Event> BroadcastPackage); //This Message Is Used To Send in a package for Broadcast
		
		Ermine::SubscriptionTicket  QueueSubscription(std::unique_ptr<EventSubscription> Subscription);
		void						DestroySubscription(Ermine::SubscriptionTicket SubscriptionTicket); //Note We Have No Way To Check Ownership Of Subscription For Now Be VERY CAREFUL...................................................
	private:
		//This Is The Storage Area We Will Store Everything Of Note Here...

		//All Different Events Are Stored In Different Containers So As To Parallilize Submitions.. Two Objects Wanting To Queue Two Different Types Of Events Need not wait ..   
		std::vector<ConcreteEvent> ConcreteEventsQueue; //These Are submitted events..
		std::recursive_mutex ConcreteEventsBufferMutex; 

		std::vector<KeyCallbackEvent> KeyCallbackEventsQueue;
		std::recursive_mutex KeyCallBackEventsBufferMutex;

		std::vector<CharacterCallbackEvent>CharacterCallbackEventsQueue;
		std::recursive_mutex CharacterCallBackEventsBufferMutex;

		std::vector<CursorPositionCallbackEvent>CursorPositionCallbackEventsQueue;
		std::recursive_mutex CursorPositionCallbackEventsBufferMutex;

		std::vector<MouseButtonCallbackEvent>MouseButtonCallbackEventsQueue;
		std::recursive_mutex MouseButtonCallbackEventsBufferMutex;

		std::vector<ScrollCallbackEvent>ScrollCallbackEventsQueue;
		std::recursive_mutex ScrollCallbackEventsBufferMutex;

		std::vector<TileSelectedEvent> TileSelectedCallbackEventsQueue;
		std::recursive_mutex TileSelectedCallbackEventsBufferMutex;

		std::vector<OnTickEvent> OnTickCallbackEventsQueue;
		std::recursive_mutex OnTickCallbackEventsBufferMutex;

		std::vector<OnBeginEvent> OnBeginEventsQueue;
		std::recursive_mutex OnBeginEventsBufferMutex;
		//Donot Forget to add destructors for these containers inside the destructor..

	private:
		//Start Store Subscriptions..//
		std::unordered_map<int,ConcreteEventSubscription> ConcreteEventSubscriptions;
		std::recursive_mutex ConcreteEventsSubscriptionsMutex;

		std::unordered_map<int,KeyCallbackEventSubscription> KeyCallbackEventsSubscriptions;
		std::recursive_mutex KeyCallbackEventsSubscriptionsMutex;

		std::unordered_map<int,CharacterCallbackEventSubscription> CharacterCallbackEventSubscriptions;
		std::recursive_mutex CharacterCallbackEventsSubscriptionsMutex;

		std::unordered_map<int,CursorPositionCallbackEventSubscription> CursorPositionCallbackEventSubscriptions;
		std::recursive_mutex CursorPositionCallbackEventsSubscriptionsMutex;

		std::unordered_map<int,MouseButtonCallbackEventSubscription> MouseButtonCallbackEventSubscriptions;
		std::recursive_mutex MouseButtonCallbackEventsSubscriptionsMutex;

		std::unordered_map<int,ScrollCallbackEventSubscription> ScrollCallbackEventSubscriptions;
		std::recursive_mutex ScrollCallbackEventsSubscriptionsMutex;

		std::unordered_map<int,TileSelectedEventSubscription> TileSelectedCallbackEventSubscriptions;
		std::recursive_mutex TileSelectedCallbackEventsSubscriptionsMutex;

		std::unordered_map<int,OnTickEventSubscription> OnTickCallbackEventSubscriptions;
		std::recursive_mutex OnTickCallbackEventSubscriptionsMutex;

		std::unordered_map<int, OnBeginEventSubscription> OnBeginEventSubscriptions;
		std::recursive_mutex OnBeginEventSubscriptionsMutex;
		//Ended Store Subscription..//

	private:
#pragma region TemplateFunctions
		template<typename SubscriptionDataTypeName>
		void QueueSubscriptionTemplate(std::recursive_mutex& Mut,std::unordered_map<int, SubscriptionDataTypeName>& SubsQ, Ermine::EventSubscription* EvePtr,SubscriptionTicket& Ticket)
		{
			std::unique_lock<std::recursive_mutex> Lock(Mut);
			SubsQ.emplace(Ticket, *((SubscriptionDataTypeName*)(EvePtr)));
		}

		template<typename SubscriptionsQueue>
		void DestorySubscriptionTemplate(std::recursive_mutex& Mut, SubscriptionsQueue& SubQ, Ermine::SubscriptionTicket SubscriptionTicket)
		{
			std::unique_lock<std::recursive_mutex> Lock(Mut);
			if(SubQ.erase(SubscriptionTicket) == 0)
				STDOUTDefaultLog_Warn("Invalid Ticket Submitted For Erasing.. Could Not Erase A Subscription");
			CloseSubscriptionTicket(SubscriptionTicket);
		}

		template<typename MutexToAcquire,typename EventsQueue,typename SubscriberQueue>
		void DispatchMessages(MutexToAcquire& MA,EventsQueue& EQ,SubscriberQueue& SQ)
		{
			std::unique_lock Lock = std::unique_lock(MA);

			for (int i = 0; i < EQ.size(); i++)
			{
				for (auto j = SQ.begin(); j != SQ.end(); j++) //for (auto& j : ConcreteEventSubscriptions)
				{
					//Start Delete Objects Which Are Marked For Deletion Right..//
					std::shared_ptr<Ermine::GeneratedObject> Subs = j->second.GetSubscribedObject();
					if (Subs != nullptr)
					{
						while (Subs->IsHandleValid() == false)
						{
							j = SQ.erase(j);
							if (j == SQ.end())
								break;
						}
						if (j == SQ.end())
							break;
						
						auto Health = Subs->GetObjectHealth();
						while (Health == Ermine::ObjectStatus::StatusMarkedForDeletion)
						{
							j = SQ.erase(j);
							if (j == SQ.end())
								break;
						}
						if (j == SQ.end())
							break;
						
						//Ended Delete Objects Which Are Marked For Deletion Right..//
						if (j->second.CanIRecieveEventFlag == true)
						{
							j->second.CallableObject(&EQ[i]);
						}

						//If The Event Is Already handled No Point In Handling it Further Right..
						if (EQ[i].IsEventHandled() == true)
							break;
					}
				}
				EQ.erase(EQ.begin() + i);
			}
		}
#pragma endregion

		//This Method Gets A Subscription Ticket If A Slot Is Available.. The Bool is also Set True Remember.. 
		int GetOpenSubscriptionTicket();
		void CloseSubscriptionTicket(int SubscriptionTicket);

	private:
		//This Container Hopefully Keeps The Subscription Tickets Given Out In Track..
		static std::vector<bool> SubscriptionTicketsInUse;

	};

}