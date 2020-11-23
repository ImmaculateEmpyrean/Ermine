#include "stdafx.h"
#include "EventBroadcastStation.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Graphics/Renderer/OpenGLErrorChecker.h"

#include<Object.h>

#pragma region StaticDeclarations
Ermine::EventBroadcastStation* Ermine::EventBroadcastStation::EventBroadcastStationPointer = nullptr; //Note The Broadcast Station is not initialized
std::once_flag Ermine::EventBroadcastStation::LazyInitializationFlag;
std::atomic<bool> Ermine::EventBroadcastStation::StationDestructionOrdered = false;
std::thread* Ermine::EventBroadcastStation::StationThreadObject;
std::vector<bool> Ermine::EventBroadcastStation::SubscriptionTicketsInUse;
#pragma endregion

void EventBroadcastStationMainRoutine()
{
	while (Ermine::EventBroadcastStation::StationDestructionOrdered == false)
	{
		//In Future Sleep Until Atleast There Is A Message In One Of The Event Buffers.. That Might Prove To Be A Wise Descision Instead Of An Arbitrary One Hundred
		_sleep(100);

		auto Station = Ermine::EventBroadcastStation::GetStation();
		Station->DispatchMessagesSuperior();
	}
}


Ermine::EventBroadcastStation::~EventBroadcastStation()
{}

Ermine::EventBroadcastStation* Ermine::EventBroadcastStation::GetStation()
{
	std::call_once(LazyInitializationFlag, []() {
		EventBroadcastStationPointer = new EventBroadcastStation(); //Create A New event Broadcast Station..
		StationThreadObject = new std::thread(EventBroadcastStationMainRoutine);

		SubscriptionTicketsInUse.resize(1000, false);
	});

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

		//There Is No Point In Having More Than One Event Tick Queued.. If The Renderer Thread Is Running At A Faster Rate Than The Message Broadcaster.. All I Can Say Is WOW.. THIS MAY CHANGE IN THE FUTURE..
		if(OnTickCallbackEventsQueue.size() == 0)
			OnTickCallbackEventsQueue.push_back(std::move(*((OnTickEvent*)(EvePtr))));
	}
	else if (BroadcastType == EventType::OnBeginEvent)
	{
		std::unique_lock<std::recursive_mutex> Lock(OnBeginEventsBufferMutex);
		OnBeginEventsQueue.push_back(std::move(*((OnBeginEvent*)(EvePtr))));
	}

	delete EvePtr;
}

Ermine::SubscriptionTicket Ermine::EventBroadcastStation::QueueSubscription(std::unique_ptr<EventSubscription> Subscription)
{
	auto SubscriptionType = Subscription->GetEventSubscriptionType();
	EventSubscription* EvePtr = Subscription.release();
	
	SubscriptionTicket Ticket = SubscriptionTicket(GetOpenSubscriptionTicket() , SubscriptionType);

	switch (SubscriptionType)
	{
	case EventType::ConcreteEvent:				 QueueSubscriptionTemplate<ConcreteEventSubscription>(ConcreteEventsSubscriptionsMutex, ConcreteEventSubscriptions, EvePtr, Ticket);
		break;
	case EventType::KeyCallbackEvent:			 QueueSubscriptionTemplate<KeyCallbackEventSubscription>(KeyCallbackEventsSubscriptionsMutex, KeyCallbackEventsSubscriptions, EvePtr, Ticket);
		break;
	case EventType::CharacterCallbackEvent:		 QueueSubscriptionTemplate<CharacterCallbackEventSubscription>(CharacterCallbackEventsSubscriptionsMutex, CharacterCallbackEventSubscriptions, EvePtr, Ticket);
		break;
	case EventType::CursorPositionCallbackEvent: QueueSubscriptionTemplate<CursorPositionCallbackEventSubscription>(CursorPositionCallbackEventsSubscriptionsMutex, CursorPositionCallbackEventSubscriptions, EvePtr, Ticket);
		break;
	case EventType::MouseButtonCallbackEvent:    QueueSubscriptionTemplate<MouseButtonCallbackEventSubscription>(MouseButtonCallbackEventsSubscriptionsMutex, MouseButtonCallbackEventSubscriptions, EvePtr, Ticket);
		break;
	case EventType::ScrollCallbackEvent:		 QueueSubscriptionTemplate<ScrollCallbackEventSubscription>(ScrollCallbackEventsSubscriptionsMutex, ScrollCallbackEventSubscriptions, EvePtr, Ticket);
		break;
	case EventType::TileSelectedEvent:			 QueueSubscriptionTemplate<TileSelectedEventSubscription>(TileSelectedCallbackEventsSubscriptionsMutex, TileSelectedCallbackEventSubscriptions, EvePtr, Ticket);
		break;
	case EventType::OnTickEvent:				 QueueSubscriptionTemplate<OnTickEventSubscription>(OnTickCallbackEventSubscriptionsMutex, OnTickCallbackEventSubscriptions, EvePtr, Ticket);
		break;
	case EventType::OnBeginEvent:				 QueueSubscriptionTemplate<OnBeginEventSubscription>(OnBeginEventSubscriptionsMutex, OnBeginEventSubscriptions, EvePtr, Ticket);
		break;
	default: STDOUTDefaultLog_Critical("Unknown Subscription Type Recieved For Queing Check Api Maybe... Dunno I should never trigger");
	}

	/*switch (SubscriptionType)
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
	case EventType::OnBeginEvent: OnBeginEventSubscriptions.emplace(Ticket, *((OnBeginEventSubscription*)(EvePtr)));
		break;
	default: STDOUTDefaultLog_Critical("Unknown Subscription Type Recieved For Queing Check Api Maybe... Dunno I should never trigger");
	}*/

	delete EvePtr;

	//Return The Ticket Back To The User Or Whatever Is Calling This..
	return std::move(Ticket);
}

void Ermine::EventBroadcastStation::DestroySubscription(Ermine::SubscriptionTicket SubscriptionTicket)
{
	auto SubscriptionType = SubscriptionTicket.GetEventSubscriptionType();

	/*if (SubscriptionType == Ermine::EventType::ConcreteEvent)
		DestorySubscriptionTemplate(ConcreteEventsSubscriptionsMutex, ConcreteEventSubscriptions, std::move(SubscriptionTicket));

	if (SubscriptionType == Ermine::EventType::KeyCallbackEvent)
		DestorySubscriptionTemplate(KeyCallbackEventsSubscriptionsMutex, KeyCallbackEventsSubscriptions, std::move(SubscriptionTicket));
	if (SubscriptionType == Ermine::EventType::CharacterCallbackEvent)
		DestorySubscriptionTemplate(CharacterCallbackEventsSubscriptionsMutex, CharacterCallbackEventSubscriptions, std::move(SubscriptionTicket));
	if (SubscriptionType == Ermine::EventType::CursorPositionCallbackEvent)
		DestorySubscriptionTemplate(CursorPositionCallbackEventsSubscriptionsMutex, CursorPositionCallbackEventSubscriptions, std::move(SubscriptionTicket));
	if (SubscriptionType == Ermine::EventType::MouseButtonCallbackEvent)
		DestorySubscriptionTemplate(MouseButtonCallbackEventsSubscriptionsMutex, MouseButtonCallbackEventSubscriptions, std::move(SubscriptionTicket));
	if (SubscriptionType == Ermine::EventType::ScrollCallbackEvent)
		DestorySubscriptionTemplate(ScrollCallbackEventsSubscriptionsMutex, ScrollCallbackEventSubscriptions, std::move(SubscriptionTicket));

	if (SubscriptionType == Ermine::EventType::TileSelectedEvent)
		DestorySubscriptionTemplate(TileSelectedCallbackEventsSubscriptionsMutex, TileSelectedCallbackEventSubscriptions, std::move(SubscriptionTicket));*/


	switch (SubscriptionType)
	{
	case EventType::ConcreteEvent:				 DestorySubscriptionTemplate(ConcreteEventsSubscriptionsMutex, ConcreteEventSubscriptions, std::move(SubscriptionTicket));
		break;
	case EventType::KeyCallbackEvent:			 DestorySubscriptionTemplate(KeyCallbackEventsSubscriptionsMutex, KeyCallbackEventsSubscriptions, std::move(SubscriptionTicket));
		break;
	case EventType::CharacterCallbackEvent:		 DestorySubscriptionTemplate(CharacterCallbackEventsSubscriptionsMutex, CharacterCallbackEventSubscriptions, std::move(SubscriptionTicket));
		break;
	case EventType::CursorPositionCallbackEvent: DestorySubscriptionTemplate(CursorPositionCallbackEventsSubscriptionsMutex, CursorPositionCallbackEventSubscriptions, std::move(SubscriptionTicket));
		break;
	case EventType::MouseButtonCallbackEvent:	 DestorySubscriptionTemplate(MouseButtonCallbackEventsSubscriptionsMutex, MouseButtonCallbackEventSubscriptions, std::move(SubscriptionTicket));
		break;
	case EventType::ScrollCallbackEvent:		 DestorySubscriptionTemplate(ScrollCallbackEventsSubscriptionsMutex, ScrollCallbackEventSubscriptions, std::move(SubscriptionTicket));
		break;
	case EventType::TileSelectedEvent:			 DestorySubscriptionTemplate(TileSelectedCallbackEventsSubscriptionsMutex, TileSelectedCallbackEventSubscriptions, std::move(SubscriptionTicket));
		break;
	case EventType::OnTickEvent:				 DestorySubscriptionTemplate(OnTickCallbackEventSubscriptionsMutex, OnTickCallbackEventSubscriptions, std::move(SubscriptionTicket));
		break;
	case EventType::OnBeginEvent:				 DestorySubscriptionTemplate(OnBeginEventSubscriptionsMutex, OnBeginEventSubscriptions, std::move(SubscriptionTicket));
		break;
	default: STDOUTDefaultLog_Critical("Unknown Subscription Type Recieved For Queing Check Api Maybe... Dunno I should never trigger");
	}


	/*if (SubscriptionType == Ermine::EventType::ConcreteEvent)
	{
		std::unique_lock<std::recursive_mutex> Lock(ConcreteEventsSubscriptionsMutex);
		ConcreteEventSubscriptions.erase(SubscriptionTicket);
		CloseSubscriptionTicket(SubscriptionTicket);
	}
	else if (SubscriptionType == Ermine::EventType::KeyCallbackEvent)
	{
		std::unique_lock<std::recursive_mutex> Lock(KeyCallbackEventsSubscriptionsMutex);
		KeyCallbackEventsSubscriptions.erase(SubscriptionTicket);
		CloseSubscriptionTicket(SubscriptionTicket);
	}
	else if (SubscriptionType == Ermine::EventType::CharacterCallbackEvent)
	{
		std::unique_lock<std::recursive_mutex> Lock(CharacterCallbackEventsSubscriptionsMutex);
		CharacterCallbackEventSubscriptions.erase(SubscriptionTicket);
		CloseSubscriptionTicket(SubscriptionTicket);
	}
	else if (SubscriptionType == Ermine::EventType::CursorPositionCallbackEvent)
	{
		std::unique_lock<std::recursive_mutex> Lock(CursorPositionCallbackEventsSubscriptionsMutex);
		CursorPositionCallbackEventSubscriptions.erase(SubscriptionTicket);
		CloseSubscriptionTicket(SubscriptionTicket);
	}
	else if (SubscriptionType == Ermine::EventType::MouseButtonCallbackEvent)
	{
		std::unique_lock<std::recursive_mutex> Lock(MouseButtonCallbackEventsSubscriptionsMutex);
		MouseButtonCallbackEventSubscriptions.erase(SubscriptionTicket);
		CloseSubscriptionTicket(SubscriptionTicket);
	}
	else if (SubscriptionType == Ermine::EventType::ScrollCallbackEvent)
	{
		std::unique_lock<std::recursive_mutex> Lock(ScrollCallbackEventsSubscriptionsMutex);
		ScrollCallbackEventSubscriptions.erase(SubscriptionTicket);
		CloseSubscriptionTicket(SubscriptionTicket);
	}
	else if (SubscriptionType == Ermine::EventType::TileSelectedEvent)
	{
		std::unique_lock<std::recursive_mutex> Lock(TileSelectedCallbackEventsSubscriptionsMutex);
		TileSelectedCallbackEventSubscriptions.erase(SubscriptionTicket);
		CloseSubscriptionTicket(SubscriptionTicket);
	}
	else if (SubscriptionType == Ermine::EventType::OnTickEvent)
	{
		std::unique_lock<std::recursive_mutex> Lock(OnTickEventCallbackEventsSubscriptionsMutex);
		OnTickCallbackEventSubscriptions.erase(SubscriptionTicket);
		CloseSubscriptionTicket(SubscriptionTicket);
	}
	else if (SubscriptionType == Ermine::EventType::OnBeginEvent)
	{
		std::unique_lock<std::recursive_mutex> Lock(OnBeginEventCallbackEventsSubscriptionsMutex);
		OnBeginEventSubscriptions.erase(SubscriptionTicket);
		CloseSubscriptionTicket(SubscriptionTicket);
	}
	else STDOUTDefaultLog_Critical("Unknown Subscription Type Recieved For Queing Check Api Maybe... Dunno I should never trigger");
	
	return;*/

	/*
	//Mutex Not Locked Here Dangerous.. Rewriting The Function..
	
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
	case EventType::OnBeginEvent: OnBeginEventSubscriptions.erase(SubscriptionTicket);
		break;
	default: STDOUTDefaultLog_Critical("Unknown Subscription Type Recieved For Queing Check Api Maybe... Dunno I should never trigger");
	}

	return;*/
}

void Ermine::EventBroadcastStation::DispatchMessagesSuperior()
{
	DispatchMessages(ConcreteEventsBufferMutex              ,ConcreteEventsQueue              ,ConcreteEventSubscriptions);
	DispatchMessages(KeyCallBackEventsBufferMutex           ,KeyCallbackEventsQueue           ,KeyCallbackEventsSubscriptions);
	DispatchMessages(CharacterCallBackEventsBufferMutex     ,CharacterCallbackEventsQueue     ,CharacterCallbackEventSubscriptions);
	DispatchMessages(CursorPositionCallbackEventsBufferMutex,CursorPositionCallbackEventsQueue,CursorPositionCallbackEventSubscriptions);
	DispatchMessages(MouseButtonCallbackEventsBufferMutex   ,MouseButtonCallbackEventsQueue   ,MouseButtonCallbackEventSubscriptions);
	DispatchMessages(ScrollCallbackEventsBufferMutex        ,ScrollCallbackEventsQueue        ,ScrollCallbackEventSubscriptions);
	DispatchMessages(TileSelectedCallbackEventsBufferMutex  ,TileSelectedCallbackEventsQueue  ,TileSelectedCallbackEventSubscriptions);
	
	DispatchMessages(OnTickCallbackEventsBufferMutex        ,OnTickCallbackEventsQueue        ,OnTickCallbackEventSubscriptions);
	DispatchMessages(OnBeginEventsBufferMutex               ,OnBeginEventsQueue               ,OnBeginEventSubscriptions);
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
