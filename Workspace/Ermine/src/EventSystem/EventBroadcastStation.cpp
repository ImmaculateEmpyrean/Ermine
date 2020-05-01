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
	Ermine::EventType BroadcastType = BroadcastPackage->GetEventType();
#pragma region OldWay
	/*if (BroadcastType == EventType::ConcreteEvent)
	{
		std::lock_guard<std::mutex> Loc(MainMutex, std::adopt_lock);
		ConcreteEventsQueue.push_back(*((ConcreteEvent*)(BroadcastPackage.release())));
	}

	else
		std::cout << "Unkown Type Got" << std::endl;*/
#pragma endregion OldWay

	std::lock_guard<std::mutex> Loc(MainMutex, std::adopt_lock);

	switch(BroadcastType)
	{
	case EventType::ConcreteEvent:ConcreteEventsQueue.push_back(*((ConcreteEvent*)(BroadcastPackage.release())));
		break;
	case EventType::KeyCallbackEvent:KeyCallbackEventsQueue.push_back(*((KeyCallbackEvent*)(BroadcastPackage.release())));
		break;
	case EventType::CharacterCallbackEvent:CharacterCallbackEventsQueue.push_back(*((CharacterCallbackEvent*)(BroadcastPackage.release())));
		break;
	case EventType::CursorPositionCallbackEvent:CursorPositionCallbackEventsQueue.push_back(*((CursorPositionCallbackEvent*)(BroadcastPackage.release())));
		break;
	case EventType::MouseButtonCallbackEvent:MouseButtonCallbackEventsQueue.push_back(*((MouseButtonCallbackEvent*)(BroadcastPackage.release())));
		break;
	case EventType::ScrollCallbackEvent:ScrollCallbackEventsQueue.push_back(*((ScrollCallbackEvent*)(BroadcastPackage.release())));
		break;
	default: STDOUTDefaultLog_Error("Unknown Event Type Recieved For QUeing Check Api Maybe... Dunno I should never trigger");
	}
}

void Ermine::EventBroadcastStation::QueueSubscription(std::unique_ptr<EventSubscription> Subscription)
{
#pragma region OldWayOfImplementation
	/*auto SubscriptionType = Subscription->GetEventSubscriptionType();
	if (SubscriptionType == EventType::ConcreteEvent)
	{
		std::lock_guard<std::mutex> Loc(MainMutex, std::adopt_lock);
		ConcreteEventSubscriptions.push_back(*((ConcreteEventSubscription*)(Subscription.release())));
	}
	else if (SubscriptionType == EventType::KeyCallbackEvent)
	{
		std::lock_guard<std::mutex> Loc(MainMutex, std::adopt_lock);
		KeyCallbackEventsSubscriptions.push_back(*((KeyCallbackEventSubscription*)(Subscription.release())));
	}
	else
		std::cout << "Unkown Type Got" << std::endl;*/
#pragma endregion OldWayofImplementation
	
	std::lock_guard<std::mutex> Loc(MainMutex, std::adopt_lock);
	auto SubscriptionType = Subscription->GetEventSubscriptionType();
	
	switch (SubscriptionType)
	{
	case EventType::ConcreteEvent : ConcreteEventSubscriptions.push_back(*((ConcreteEventSubscription*)(Subscription.release())));
		break;
	case EventType::KeyCallbackEvent : KeyCallbackEventsSubscriptions.push_back(*((KeyCallbackEventSubscription*)(Subscription.release())));
		break;
	case EventType::CharacterCallbackEvent: CharacterCallbackEventSubscriptions.push_back(*((CharacterCallbackEventSubscription*)(Subscription.release())));
		break;
	case EventType::CursorPositionCallbackEvent: CursorPositionCallbackEventSubscriptions.push_back(*((CursorPositionCallbackEventSubscription*)(Subscription.release())));
		break;
	case EventType::MouseButtonCallbackEvent: MouseButtonCallbackEventSubscriptions.push_back(*((MouseButtonCallbackEventSubscription*)(Subscription.release())));
		break;
	case EventType::ScrollCallbackEvent: ScrollCallbackEventSubscriptions.push_back(*((ScrollCallbackEventSubscription*)(Subscription.release())));
		break;
	default: STDOUTDefaultLog_Error("Unknown Subscription Type Recieved For QUeing Check Api Maybe... Dunno I should never trigger");
	}
}



void Ermine::EventBroadcastStation::DispatchMessages()
{
	std::lock_guard<std::mutex> Loc(MainMutex, std::adopt_lock);
	
	//Just Iterate Over All Buffers And Try To Dispatch The Messages...
	DispatchConcreteMessages();
	DispatchKeyCallbackMessages();
	DispatchCharacterCallbackMessages();
	DispatchCursorPositionCallbackMessages();
	DispatchMouseButtonCallbackMessages();
	DispatchScrollCallbackMessages();
}

void Ermine::EventBroadcastStation::DispatchConcreteMessages()
{
	for (int i = 0; i < ConcreteEventsQueue.size(); i++)
	{
		for (auto j : ConcreteEventSubscriptions)
		{
			if (j.CanIRecieveEventFlag == true)
			{
				j.CallableObject(&ConcreteEventsQueue[i]);
			}
			//If The Event Is Already handled No Point In Handling it Further Right..
			if (ConcreteEventsQueue[i].IsEventHandled() == true)
			{
				break;
			}
		}
		ConcreteEventsQueue.erase(ConcreteEventsQueue.begin() + i);
	}
}

void Ermine::EventBroadcastStation::DispatchKeyCallbackMessages()
{
	for (int i = 0; i < KeyCallbackEventsQueue.size(); i++)
	{
		for (auto j : KeyCallbackEventsSubscriptions)
		{
			if (j.CanIRecieveEventFlag == true)
			{
				j.CallableObject(&KeyCallbackEventsQueue[i]);
			}
			//If The Event Is Already handled No Point In Handling it Further Right..
			if (KeyCallbackEventsQueue[i].IsEventHandled() == true)
			{
				break;
			}
		}
		KeyCallbackEventsQueue.erase(KeyCallbackEventsQueue.begin() + i);
	}
}

void Ermine::EventBroadcastStation::DispatchCharacterCallbackMessages()
{
	for (int i = 0; i < CharacterCallbackEventsQueue.size(); i++)
	{
		for (auto j : CharacterCallbackEventSubscriptions)
		{
			if (j.CanIRecieveEventFlag == true)
			{
				j.CallableObject(&CharacterCallbackEventsQueue[i]);
			}
			//If The Event Is Already handled No Point In Handling it Further Right..
			if (CharacterCallbackEventsQueue[i].IsEventHandled() == true)
			{
				break;
			}
		}
		CharacterCallbackEventsQueue.erase(CharacterCallbackEventsQueue.begin() + i);
	}
}

void Ermine::EventBroadcastStation::DispatchCursorPositionCallbackMessages()
{
	for (int i = 0; i < CursorPositionCallbackEventsQueue.size(); i++)
	{
		for (auto j : CursorPositionCallbackEventSubscriptions)
		{
			if (j.CanIRecieveEventFlag == true)
			{
				j.CallableObject(&CursorPositionCallbackEventsQueue[i]);
			}
			//If The Event Is Already handled No Point In Handling it Further Right..
			if (CursorPositionCallbackEventsQueue[i].IsEventHandled() == true)
			{
				break;
			}
		}
		CursorPositionCallbackEventsQueue.erase(CursorPositionCallbackEventsQueue.begin() + i);
	}
}

void Ermine::EventBroadcastStation::DispatchMouseButtonCallbackMessages()
{
	for (int i = 0; i < MouseButtonCallbackEventsQueue.size(); i++)
	{
		for (auto j : MouseButtonCallbackEventSubscriptions)
		{
			if (j.CanIRecieveEventFlag == true)
			{
				j.CallableObject(&MouseButtonCallbackEventsQueue[i]);
			}
			//If The Event Is Already handled No Point In Handling it Further Right..
			if (MouseButtonCallbackEventsQueue[i].IsEventHandled() == true)
			{
				break;
			}
		}
		MouseButtonCallbackEventsQueue.erase(MouseButtonCallbackEventsQueue.begin() + i);
	}
}

void Ermine::EventBroadcastStation::DispatchScrollCallbackMessages()
{
	for (int i = 0; i < ScrollCallbackEventsQueue.size(); i++)
	{
		for (auto j : ScrollCallbackEventSubscriptions)
		{
			if (j.CanIRecieveEventFlag == true)
			{
				j.CallableObject(&ScrollCallbackEventsQueue[i]);
			}
			//If The Event Is Already handled No Point In Handling it Further Right..
			if (ScrollCallbackEventsQueue[i].IsEventHandled() == true)
			{
				break;
			}
		}
		ScrollCallbackEventsQueue.erase(ScrollCallbackEventsQueue.begin() + i);
	}
}
