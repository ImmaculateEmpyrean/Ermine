#include "stdafx.h"
#include "EventBroadcastStation.h"

Ermine::EventBroadcastStation* Ermine::EventBroadcastStation::EventBroadcastStationPointer = nullptr; //Note The Broadcast Station is not initialized
std::once_flag Ermine::EventBroadcastStation::LazyInitializationFlag;
std::atomic<bool> Ermine::EventBroadcastStation::StationDestructionOrdered = false;
std::thread* Ermine::EventBroadcastStation::StationThreadObject;
std::mutex Ermine::EventBroadcastStation::MainMutex;

std::vector<bool> Ermine::EventBroadcastStation::SubscriptionTicketsInUse;

void EventBroadcastStationMainRoutine()
{
	while (Ermine::EventBroadcastStation::StationDestructionOrdered == false)
	{
		_sleep(100);
		auto Station = Ermine::EventBroadcastStation::GetStation();
		Station->DispatchMessages();
	}
}


Ermine::EventBroadcastStation::~EventBroadcastStation()
{}

Ermine::EventBroadcastStation* Ermine::EventBroadcastStation::GetStation()
{
	//std::lock_guard<std::mutex> GetStationLockGaurd(GetStationLock); //This Ensures Only One Thread Can Access The Retuen At a Given Time
	
	std::call_once(LazyInitializationFlag, []() {
		EventBroadcastStationPointer = new EventBroadcastStation(); //Create A New event Broadcast Station..
		StationThreadObject = new std::thread(EventBroadcastStationMainRoutine);

		SubscriptionTicketsInUse.resize(1000, false);
	});

	std::unique_lock<std::mutex> Loc(MainMutex);//,std::adopt_lock);
	return EventBroadcastStationPointer;
}

void Ermine::EventBroadcastStation::DestroyStation()
{
	StationDestructionOrdered = true;
	if (EventBroadcastStationPointer != nullptr)
	{
		if (StationThreadObject != nullptr)
		{
			StationThreadObject->join();
			delete StationThreadObject;
		}
		delete EventBroadcastStationPointer;
	}
}

void Ermine::EventBroadcastStation::QueueBroadcast(std::unique_ptr<Event> BroadcastPackage)
{
	std::unique_lock<std::mutex> Loc(MainMutex);//, std::adopt_lock);//std::lock_guard<std::mutex> Loc(MainMutex,std::adopt_lock);
	
	Ermine::EventType BroadcastType = BroadcastPackage->GetEventType();
	Event* EvePtr = BroadcastPackage.release();

	switch(BroadcastType)
	{
	case EventType::ConcreteEvent:ConcreteEventsQueue.push_back(*((ConcreteEvent*)(EvePtr)));
		break;
	case EventType::KeyCallbackEvent:KeyCallbackEventsQueue.push_back(*((KeyCallbackEvent*)(EvePtr)));
		break;
	case EventType::CharacterCallbackEvent:CharacterCallbackEventsQueue.push_back(*((CharacterCallbackEvent*)(EvePtr)));
		break;
	case EventType::CursorPositionCallbackEvent:CursorPositionCallbackEventsQueue.push_back(*((CursorPositionCallbackEvent*)(EvePtr)));
		break;
	case EventType::MouseButtonCallbackEvent:MouseButtonCallbackEventsQueue.push_back(*((MouseButtonCallbackEvent*)(EvePtr)));
		break;
	case EventType::ScrollCallbackEvent:ScrollCallbackEventsQueue.push_back(*((ScrollCallbackEvent*)(EvePtr)));
		break;
	case EventType::TileSelectedEvent: TileSelectedCallbackEventsQueue.push_back(*((TileSelectedEvent*)(EvePtr)));
		break;
	case EventType::OnTickEvent: OnTickCallbackEventsQueue.push_back(*((OnTickEvent*)(EvePtr)));
		break; 
	default: STDOUTDefaultLog_Critical("Unknown Event Type Recieved For QUeing Check Api Maybe... Dunno I should never trigger");
	}
	delete EvePtr;
}

Ermine::SubscriptionTicket Ermine::EventBroadcastStation::QueueSubscription(std::unique_ptr<EventSubscription> Subscription)
{
	std::unique_lock<std::mutex> Loc(MainMutex);
	
	auto SubscriptionType = Subscription->GetEventSubscriptionType();
	EventSubscription* EvePtr = Subscription.release();
	
	SubscriptionTicket Ticket = SubscriptionTicket(GetOpenSubscriptionTicket() , SubscriptionType);

	switch (SubscriptionType)
	{
	case EventType::ConcreteEvent : ConcreteEventSubscriptions.emplace(Ticket,*((ConcreteEventSubscription*)(EvePtr)));								
		break;
	case EventType::KeyCallbackEvent : KeyCallbackEventsSubscriptions.emplace(Ticket,*((KeyCallbackEventSubscription*)(EvePtr)));
		break;
	case EventType::CharacterCallbackEvent: CharacterCallbackEventSubscriptions.emplace(Ticket,*((CharacterCallbackEventSubscription*)(EvePtr)));
		break;
	case EventType::CursorPositionCallbackEvent: CursorPositionCallbackEventSubscriptions.emplace(Ticket,*((CursorPositionCallbackEventSubscription*)(EvePtr)));
		break;
	case EventType::MouseButtonCallbackEvent: MouseButtonCallbackEventSubscriptions.emplace(Ticket,*((MouseButtonCallbackEventSubscription*)(EvePtr)));
		break;
	case EventType::ScrollCallbackEvent: ScrollCallbackEventSubscriptions.emplace(Ticket,*((ScrollCallbackEventSubscription*)(EvePtr)));
		break;
	case EventType::TileSelectedEvent: TileSelectedCallbackEventSubscriptions.emplace(Ticket,*((TileSelectedEventSubscription*)(EvePtr)));
		break;
	case EventType::OnTickEvent: OnTickCallbackEventSubscriptions.emplace(Ticket,*((OnTickEventSubscription*)(EvePtr)));
		break;
	default: STDOUTDefaultLog_Critical("Unknown Subscription Type Recieved For Queing Check Api Maybe... Dunno I should never trigger");
	}

	delete EvePtr;

	//Return The Ticket Back To The User Or Whatever Is Calling This..
	return std::move(Ticket);
}

void Ermine::EventBroadcastStation::DestroySubscription(Ermine::SubscriptionTicket SubscriptionTicket)
{
	
	std::unique_lock<std::mutex> Loc(MainMutex);

	auto SubscriptionType = SubscriptionTicket.GetEventSubscriptionType();
	switch (SubscriptionType)
	{
	case EventType::ConcreteEvent: ConcreteEventSubscriptions.erase(SubscriptionTicket);
		break;
	case EventType::KeyCallbackEvent: KeyCallbackEventsSubscriptions.erase(SubscriptionTicket);
		break;
	case EventType::CharacterCallbackEvent: CharacterCallbackEventSubscriptions.erase(SubscriptionTicket);
		break;
	case EventType::CursorPositionCallbackEvent: CursorPositionCallbackEventSubscriptions.erase(SubscriptionTicket);
		break;
	case EventType::MouseButtonCallbackEvent: MouseButtonCallbackEventSubscriptions.erase(SubscriptionTicket);
		break;
	case EventType::ScrollCallbackEvent: ScrollCallbackEventSubscriptions.erase(SubscriptionTicket);
		break;
	case EventType::TileSelectedEvent: TileSelectedCallbackEventSubscriptions.erase(SubscriptionTicket);
		break;
	case EventType::OnTickEvent: OnTickCallbackEventSubscriptions.erase(SubscriptionTicket);
		break;
	default: STDOUTDefaultLog_Critical("Unknown Subscription Type Recieved For Queing Check Api Maybe... Dunno I should never trigger");
	}

	CloseSubscriptionTicket(SubscriptionTicket);

	return; //This Must Return To The User Right..
	//Also I Think The Mutex Is Way Too Coarse Grained.. Maybe Fix It A Bit In The Future
}



void Ermine::EventBroadcastStation::DispatchMessages()
{
	std::unique_lock<std::mutex> Loc(MainMutex);//std::lock_guard<std::mutex> Loc(MainMutex, std::adopt_lock);
	
	//Just Iterate Over All Buffers And Try To Dispatch The Messages...
	DispatchConcreteMessages();
	DispatchKeyCallbackMessages();
	DispatchCharacterCallbackMessages();
	DispatchCursorPositionCallbackMessages();
	DispatchMouseButtonCallbackMessages();
	DispatchScrollCallbackMessages();
	DispatchTileSelectedCallbackMessages();
	DispatchOnTickCallbackMessages();
}

void Ermine::EventBroadcastStation::DispatchConcreteMessages()
{
	for (int i = 0; i < ConcreteEventsQueue.size(); i++)
	{
		int c = 0;
		for (auto j : ConcreteEventSubscriptions)
		{
			if (j.second.CanIRecieveEventFlag == true)
			{
				j.second.CallableObject(&ConcreteEventsQueue[i]);
			}
			c++;

			//If The Event Is Already handled No Point In Handling it Further Right..
			if (ConcreteEventsQueue[i].IsEventHandled() == true)
				break;
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
			if (j.second.CanIRecieveEventFlag == true)
			{
				j.second.CallableObject(&KeyCallbackEventsQueue[i]);
			}
			//If The Event Is Already handled No Point In Handling it Further Right..
			if (KeyCallbackEventsQueue[i].IsEventHandled() == true)
				break;
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
			if (j.second.CanIRecieveEventFlag == true)
			{
				j.second.CallableObject(&CharacterCallbackEventsQueue[i]);
			}
			//If The Event Is Already handled No Point In Handling it Further Right..
			if (CharacterCallbackEventsQueue[i].IsEventHandled() == true)
				break;
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
			if (j.second.CanIRecieveEventFlag == true)
			{
				j.second.CallableObject(&CursorPositionCallbackEventsQueue[i]);
			}
			//If The Event Is Already handled No Point In Handling it Further Right..
			if (CursorPositionCallbackEventsQueue[i].IsEventHandled() == true)
				break;
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
			if (j.second.CanIRecieveEventFlag == true)
			{
				j.second.CallableObject(&MouseButtonCallbackEventsQueue[i]);
			}
			//If The Event Is Already handled No Point In Handling it Further Right..
			if (MouseButtonCallbackEventsQueue[i].IsEventHandled() == true)
				break;
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
			if (j.second.CanIRecieveEventFlag == true)
			{
				j.second.CallableObject(&ScrollCallbackEventsQueue[i]);
			}
			//If The Event Is Already handled No Point In Handling it Further Right..
			if (ScrollCallbackEventsQueue[i].IsEventHandled() == true)
				break;
		}
		ScrollCallbackEventsQueue.erase(ScrollCallbackEventsQueue.begin() + i);
	}
}

void Ermine::EventBroadcastStation::DispatchTileSelectedCallbackMessages()
{
	for (int i = 0; i < TileSelectedCallbackEventsQueue.size(); i++)
	{
		for (auto j : TileSelectedCallbackEventSubscriptions)
		{
			if (j.second.CanIRecieveEventFlag == true)
			{
				j.second.CallableObject(&TileSelectedCallbackEventsQueue[i]);
			}
			//If The Event Is Already handled No Point In Handling it Further Right..
			if (TileSelectedCallbackEventsQueue[i].IsEventHandled() == true)
				break;
		}
		TileSelectedCallbackEventsQueue.erase(TileSelectedCallbackEventsQueue.begin() + i);
	}
}

void Ermine::EventBroadcastStation::DispatchOnTickCallbackMessages()
{
	for (int i = 0; i < OnTickCallbackEventsQueue.size(); i++)
	{
		for (auto j : OnTickCallbackEventSubscriptions)
		{
			if (j.second.CanIRecieveEventFlag == true)
			{
				j.second.CallableObject(&OnTickCallbackEventsQueue[i]);
			}
			//If The Event Is Already handled No Point In Handling it Further Right..
			if (OnTickCallbackEventsQueue[i].IsEventHandled() == true)
				break;
		}
		OnTickCallbackEventsQueue.erase(OnTickCallbackEventsQueue.begin() + i);
	}
}

static std::mutex SubscriptionTicketVectorMutex;

int Ermine::EventBroadcastStation::GetOpenSubscriptionTicket()
{
	//To See That To Threads Dont Crash Headfirst into each other..
	std::lock_guard<std::mutex> TicketGaurd(SubscriptionTicketVectorMutex);

	//This Seek Variable Is Used to Make The Function In Question A TAd Bit Faster..
	static int Seek = 0;

	for (int i = Seek; i < SubscriptionTicketsInUse.size(); i++)
	{
		if (SubscriptionTicketsInUse[i] == false)
		{
			Seek = i + 1;
			SubscriptionTicketsInUse[i] = true;
			return i;
		}
	}

	Seek = 0;

	//Try To Again A Subscription This Time From Start To Finish.. Could Use A Simple Goto But MEH Whatever
	for (int i = Seek; i < SubscriptionTicketsInUse.size(); i++)
	{
		if (SubscriptionTicketsInUse[i] == false)
		{
			Seek = i + 1;
			SubscriptionTicketsInUse[i] = true;
			return i;
		}
	}

	//Worst Case Create A Space And Return.. This Will Be Abysmally Slow In RealTime..
	SubscriptionTicketsInUse.emplace_back(true);
	return SubscriptionTicketsInUse.size() - 1;
}

void Ermine::EventBroadcastStation::CloseSubscriptionTicket(int SubscriptionTicket)
{
	std::lock_guard<std::mutex> TicketGaurd(SubscriptionTicketVectorMutex);

	if (SubscriptionTicketsInUse[SubscriptionTicket] == true)
		SubscriptionTicketsInUse[SubscriptionTicket] = false;
	else
	{
		STDOUTDefaultLog_Critical("Tried To Close A Subscription Ticket Which Was Not Even Issued.. There Must Be Some error Somewhere Please See To It Very Urgently.. ");
	}
}
