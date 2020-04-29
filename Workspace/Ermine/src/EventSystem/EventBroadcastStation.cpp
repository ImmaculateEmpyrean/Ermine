#include "stdafx.h"
#include "EventBroadcastStation.h"

Ermine::EventBroadcastStation* Ermine::EventBroadcastStation::EventBroadcastStationPointer = nullptr; //Note The Broadcast Station is not initialized
std::once_flag Ermine::EventBroadcastStation::LazyInitializationFlag;

//std::mutex Ermine::EventBroadcastStation::GetStationLock;
std::mutex Ermine::EventBroadcastStation::MainMutex;

void EventBroadcastStationMainRoutine()
{
	_sleep(1000);

	while (true)
	{
		_sleep(100);
		std::lock_guard<std::mutex> Loc(Ermine::EventBroadcastStation::MainMutex,std::adopt_lock);
		//std::cout << "Woke up from Sleep" << std::endl;
		auto Station = Ermine::EventBroadcastStation::GetStation();
		Station->DispatchMessages();
	}
}


Ermine::EventBroadcastStation* Ermine::EventBroadcastStation::GetStation()
{
	//std::lock_guard<std::mutex> GetStationLockGaurd(GetStationLock); //This Ensures Only One Thread Can Access The Retuen At a Given Time
	

	std::call_once(LazyInitializationFlag, []() {
		EventBroadcastStationPointer = new EventBroadcastStation(); //Create A New event Broadcast Station..
		std::thread NewThreadObject(EventBroadcastStationMainRoutine);
		NewThreadObject.detach(); //This Will Run Indefinitely I Guess No Need To Bother about It I think
	});

	std::lock_guard<std::mutex> Loc(MainMutex,std::adopt_lock);
	return EventBroadcastStationPointer;
}

void Ermine::EventBroadcastStation::QueueBroadcast(std::unique_ptr<Event> BroadcastPackage)
{
	auto BroadcastType = BroadcastPackage->GetEventType();
	if (BroadcastType == EventType::ConcreteEvent)
	{
		std::lock_guard<std::mutex> Loc(MainMutex, std::adopt_lock);
		ConcreteEventsQueue.push_back(*((ConcreteEvent*)(BroadcastPackage.release())));
	}

	else
		std::cout << "Unkown Type Got" << std::endl;
}

void Ermine::EventBroadcastStation::QueueSubscription(std::unique_ptr<EventSubscription> Subscription)
{
	auto SubscriptionType = Subscription->GetEventSubscriptionType();
	if (SubscriptionType == EventType::ConcreteEvent)
	{
		std::lock_guard<std::mutex> Loc(MainMutex, std::adopt_lock);
		ConcreteEventSubscriptions.push_back(*((ConcreteEventSubscription*)(Subscription.release())));
	}

	else
		std::cout << "Unkown Type Got" << std::endl;
}

void Ermine::EventBroadcastStation::DispatchMessages()
{
	std::lock_guard<std::mutex> Loc(MainMutex, std::adopt_lock);

	for (int i = 0; i < ConcreteEventsQueue.size(); i++)
	{
		for (auto j : ConcreteEventSubscriptions)
		{
			if (j.CanIRecieveEventFlag == true)
			{
				j.CallableObject(&ConcreteEventsQueue[i]);
			}
			//j.CallableObject(*(ConcreteEventsQueue[i]));
		}
		ConcreteEventsQueue.erase(ConcreteEventsQueue.begin() + i);
	}
}
