#include "stdafx.h"
#include "EventBroadcastStation.h"

#include<Object.h>

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
		Station->DispatchMessagesSuperior();
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

	//Grab The Associated Mutex And Then Access The Required Buffer..

	if (BroadcastType == EventType::ConcreteEvent)
	{
		std::unique_lock<std::recursive_mutex> Lock(ConcreteEventsBufferMutex);
		ConcreteEventsQueue.push_back(std::move(*((ConcreteEvent*)(EvePtr))));
	}
	else if (BroadcastType == EventType::KeyCallbackEvent)
	{
		std::unique_lock<std::recursive_mutex> Lock(KeyCallBackEventsBufferMutex);
		KeyCallbackEventsQueue.push_back(std::move(*((KeyCallbackEvent*)(EvePtr))));
	}
	else if (BroadcastType == EventType::CharacterCallbackEvent)
	{
		std::unique_lock<std::recursive_mutex> Lock(CharacterCallBackEventsBufferMutex);
		CharacterCallbackEventsQueue.push_back(std::move(*((CharacterCallbackEvent*)(EvePtr))));
	}
	else if (BroadcastType == EventType::CursorPositionCallbackEvent)
	{
		std::unique_lock<std::recursive_mutex> Lock(CursorPositionCallbackEventsBufferMutex);
		CursorPositionCallbackEventsQueue.push_back(std::move(*((CursorPositionCallbackEvent*)(EvePtr))));
	}
	else if (BroadcastType == EventType::MouseButtonCallbackEvent)
	{
		std::unique_lock<std::recursive_mutex> Lock(MouseButtonCallbackEventsBufferMutex);
		MouseButtonCallbackEventsQueue.push_back(std::move(*((MouseButtonCallbackEvent*)(EvePtr))));
	}
	else if (BroadcastType == EventType::ScrollCallbackEvent)
	{
		std::unique_lock<std::recursive_mutex> Lock(ScrollCallbackEventsBufferMutex);
		ScrollCallbackEventsQueue.push_back(std::move(*((ScrollCallbackEvent*)(EvePtr))));
	}
	else if (BroadcastType == EventType::TileSelectedEvent)
	{
		std::unique_lock<std::recursive_mutex> Lock(TileSelectedCallbackEventsBufferMutex);
		TileSelectedCallbackEventsQueue.push_back(std::move(*((TileSelectedEvent*)(EvePtr))));
	}
	else if (BroadcastType == EventType::OnTickEvent)
	{
		std::unique_lock<std::recursive_mutex> Lock(OnTickCallbackEventsBufferMutex);
		OnTickCallbackEventsQueue.push_back(std::move(*((OnTickEvent*)(EvePtr))));
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

void Ermine::EventBroadcastStation::DispatchMessagesSuperior()
{
	std::unique_lock<std::mutex> Loc(MainMutex);//std::lock_guard<std::mutex> Loc(MainMutex, std::adopt_lock);

	/*//Just Iterate Over All Buffers And Try To Dispatch The Messages...
	{
		std::unique_lock Lock(ConcreteEventsBufferMutex);
		DispatchConcreteMessages();
	}

	{
		std::unique_lock Lock(KeyCallBackEventsBufferMutex);
		DispatchKeyCallbackMessages();
	}

	{
		std::unique_lock Lock(CharacterCallBackEventsBufferMutex);
		DispatchCharacterCallbackMessages();
	}

	{
		std::unique_lock Lock(CursorPositionCallbackEventsBufferMutex);
		DispatchCursorPositionCallbackMessages();
	}

	{
		std::unique_lock Lock(MouseButtonCallbackEventsBufferMutex);
		DispatchMouseButtonCallbackMessages();
	}

	{
		std::unique_lock Lock(ScrollCallbackEventsBufferMutex);
		DispatchScrollCallbackMessages();
	}

	{
		std::unique_lock Lock(TileSelectedCallbackEventsBufferMutex);
		DispatchTileSelectedCallbackMessages();
	}

	{
		std::unique_lock Lock(OnTickCallbackEventsBufferMutex);
		DispatchOnTickCallbackMessages();
	}*/

	DispatchMessages(ConcreteEventsBufferMutex              ,ConcreteEventsQueue              ,ConcreteEventSubscriptions);
	DispatchMessages(KeyCallBackEventsBufferMutex           ,KeyCallbackEventsQueue           ,KeyCallbackEventsSubscriptions);
	DispatchMessages(CharacterCallBackEventsBufferMutex     ,CharacterCallbackEventsQueue     ,CharacterCallbackEventSubscriptions);
	DispatchMessages(CursorPositionCallbackEventsBufferMutex,CursorPositionCallbackEventsQueue,CursorPositionCallbackEventSubscriptions);
	DispatchMessages(MouseButtonCallbackEventsBufferMutex   ,MouseButtonCallbackEventsQueue   ,MouseButtonCallbackEventSubscriptions);
	DispatchMessages(ScrollCallbackEventsBufferMutex        ,ScrollCallbackEventsQueue        ,ScrollCallbackEventSubscriptions);
	DispatchMessages(TileSelectedCallbackEventsBufferMutex  ,TileSelectedCallbackEventsQueue  ,TileSelectedCallbackEventSubscriptions);
	DispatchMessages(OnTickCallbackEventsBufferMutex        ,OnTickCallbackEventsQueue        ,OnTickCallbackEventSubscriptions);
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
