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
		//_sleep(100);

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
	else if (BroadcastType == EventType::OnRenderTickEvent)
	{
		std::unique_lock<std::recursive_mutex> Lock(OnRenderTickCallbackEventsBufferMutex);

		//There Is No Point In Having More Than One Event Tick Queued.. If The Renderer Thread Is Running At A Faster Rate Than The Message Broadcaster.. All I Can Say Is WOW.. THIS MAY CHANGE IN THE FUTURE..
		if(OnRenderTickCallbackEventsQueue.size() == 0)
			OnRenderTickCallbackEventsQueue.push_back(std::move(*((OnRenderTickEvent*)(EvePtr))));
	}
	else if (BroadcastType == EventType::OnUpdateTickEvent)
	{
		std::unique_lock<std::recursive_mutex> Lock(OnUpdateTickCallbackEventsBufferMutex);

		//There Is No Point In Having More Than One Event Tick Queued.. If The Renderer Thread Is Running At A Faster Rate Than The Message Broadcaster.. All I Can Say Is WOW.. THIS MAY CHANGE IN THE FUTURE..
		if (OnUpdateTickCallbackEventsQueue.size() == 0)
			OnUpdateTickCallbackEventsQueue.push_back(std::move(*((OnUpdateTickEvent*)(EvePtr))));
	}
	else if (BroadcastType == EventType::OnBeginEvent)
	{
		std::unique_lock<std::recursive_mutex> Lock(OnBeginEventsBufferMutex);
		OnBeginEventsQueue.push_back(std::move(*((OnBeginEvent*)(EvePtr))));
	}

	delete EvePtr;
}
void Ermine::EventBroadcastStation::BlockingBroadcast(std::unique_ptr<Event> BroadcastPackage)
{
	Ermine::EventType BroadcastType = BroadcastPackage->GetEventType();
	
	if (BroadcastType == EventType::ConcreteEvent)
		DispatchMessageBlocking< std::unordered_map<int, ConcreteEventSubscription>, Ermine::ConcreteEvent>(ConcreteEventSubscriptions, ConcreteEventsSubscriptionsMutex, std::move(BroadcastPackage));
	else if (BroadcastType == EventType::KeyCallbackEvent)
		DispatchMessageBlocking< std::unordered_map<int, KeyCallbackEventSubscription>, Ermine::KeyCallbackEvent>(KeyCallbackEventsSubscriptions, KeyCallbackEventsSubscriptionsMutex, std::move(BroadcastPackage));
	else if (BroadcastType == EventType::CharacterCallbackEvent)
		DispatchMessageBlocking < std::unordered_map<int, CharacterCallbackEventSubscription>, Ermine::CharacterCallbackEvent > (CharacterCallbackEventSubscriptions, CharacterCallbackEventsSubscriptionsMutex, std::move(BroadcastPackage));
	else if (BroadcastType == EventType::CursorPositionCallbackEvent)
		DispatchMessageBlocking< std::unordered_map<int, CursorPositionCallbackEventSubscription>, Ermine::CursorPositionCallbackEvent>(CursorPositionCallbackEventSubscriptions, CursorPositionCallbackEventsSubscriptionsMutex, std::move(BroadcastPackage));
	else if (BroadcastType == EventType::MouseButtonCallbackEvent)
		DispatchMessageBlocking< std::unordered_map<int, MouseButtonCallbackEventSubscription>, Ermine::MouseButtonCallbackEvent>(MouseButtonCallbackEventSubscriptions, MouseButtonCallbackEventsSubscriptionsMutex,std::move(BroadcastPackage));
	else if (BroadcastType == EventType::ScrollCallbackEvent)
		DispatchMessageBlocking< std::unordered_map<int, ScrollCallbackEventSubscription>, Ermine::ScrollCallbackEvent>(ScrollCallbackEventSubscriptions, ScrollCallbackEventsSubscriptionsMutex, std::move(BroadcastPackage));
	else if (BroadcastType == EventType::TileSelectedEvent)
		DispatchMessageBlocking< std::unordered_map<int, TileSelectedEventSubscription>, Ermine::TileSelectedEvent>(TileSelectedCallbackEventSubscriptions, TileSelectedCallbackEventsSubscriptionsMutex, std::move(BroadcastPackage));
	else if (BroadcastType == EventType::OnRenderTickEvent)
		DispatchMessageBlocking< std::unordered_map<int, OnRenderTickEventSubscription>, Ermine::OnRenderTickEvent>(OnRenderTickCallbackEventSubscriptions, OnRenderTickCallbackEventSubscriptionsMutex, std::move(BroadcastPackage));
	else if (BroadcastType == EventType::OnUpdateTickEvent)
		DispatchMessageBlocking< std::unordered_map<int, OnUpdateTickEventSubscription>, Ermine::OnUpdateTickEvent>(OnUpdateTickCallbackEventSubscriptions, OnUpdateTickCallbackEventSubscriptionsMutex, std::move(BroadcastPackage));
	else if (BroadcastType == EventType::OnBeginEvent)
		DispatchMessageBlocking< std::unordered_map<int, OnBeginEventSubscription>, Ermine::OnBeginEvent>(OnBeginEventSubscriptions, OnBeginEventSubscriptionsMutex, std::move(BroadcastPackage));
	else
	{
		STDOUTDefaultLog_Error("Invalid EventType Recieved In BlockingBroadcast.. As Such Cannot Process The Message.. Dropped Said Message. Message Type : {0}",((int)BroadcastType));
	}	
	return;
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
	case EventType::OnRenderTickEvent:				 QueueSubscriptionTemplate<OnRenderTickEventSubscription>(OnRenderTickCallbackEventSubscriptionsMutex, OnRenderTickCallbackEventSubscriptions, EvePtr, Ticket);
		break;
	case EventType::OnUpdateTickEvent:				 QueueSubscriptionTemplate<OnUpdateTickEventSubscription>(OnUpdateTickCallbackEventSubscriptionsMutex, OnUpdateTickCallbackEventSubscriptions, EvePtr, Ticket);
		break;
	case EventType::OnBeginEvent:				 QueueSubscriptionTemplate<OnBeginEventSubscription>(OnBeginEventSubscriptionsMutex, OnBeginEventSubscriptions, EvePtr, Ticket);
		break;
	default: STDOUTDefaultLog_Critical("Unknown Subscription Type Recieved For Queing Check Api Maybe... Dunno I should never trigger");
	}
	delete EvePtr;

	//Return The Ticket Back To The User Or Whatever Is Calling This..
	return std::move(Ticket);
}

void Ermine::EventBroadcastStation::DestroySubscription(Ermine::SubscriptionTicket SubscriptionTicket)
{
	auto SubscriptionType = SubscriptionTicket.GetEventSubscriptionType();

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
	case EventType::OnRenderTickEvent:				 DestorySubscriptionTemplate(OnRenderTickCallbackEventSubscriptionsMutex, OnRenderTickCallbackEventSubscriptions, std::move(SubscriptionTicket));
		break;
	case EventType::OnUpdateTickEvent:				 DestorySubscriptionTemplate(OnUpdateTickCallbackEventSubscriptionsMutex, OnUpdateTickCallbackEventSubscriptions, std::move(SubscriptionTicket));
		break;
	case EventType::OnBeginEvent:				 DestorySubscriptionTemplate(OnBeginEventSubscriptionsMutex, OnBeginEventSubscriptions, std::move(SubscriptionTicket));
		break;
	default: STDOUTDefaultLog_Critical("Unknown Subscription Type Recieved For Queing Check Api Maybe... Dunno I should never trigger");
	}
}

void Ermine::EventBroadcastStation::DispatchMessagesSuperior()
{
	DispatchMessages(ConcreteEventsBufferMutex              ,ConcreteEventsQueue              ,ConcreteEventSubscriptions			   ,ConcreteEventsSubscriptionsMutex);
	DispatchMessages(KeyCallBackEventsBufferMutex           ,KeyCallbackEventsQueue           ,KeyCallbackEventsSubscriptions		   ,KeyCallbackEventsSubscriptionsMutex);
	DispatchMessages(CharacterCallBackEventsBufferMutex     ,CharacterCallbackEventsQueue     ,CharacterCallbackEventSubscriptions	   ,CharacterCallbackEventsSubscriptionsMutex);
	DispatchMessages(CursorPositionCallbackEventsBufferMutex,CursorPositionCallbackEventsQueue,CursorPositionCallbackEventSubscriptions,CursorPositionCallbackEventsSubscriptionsMutex);
	DispatchMessages(MouseButtonCallbackEventsBufferMutex   ,MouseButtonCallbackEventsQueue   ,MouseButtonCallbackEventSubscriptions   ,MouseButtonCallbackEventsSubscriptionsMutex);
	DispatchMessages(ScrollCallbackEventsBufferMutex        ,ScrollCallbackEventsQueue        ,ScrollCallbackEventSubscriptions		   ,ScrollCallbackEventsSubscriptionsMutex);
	DispatchMessages(TileSelectedCallbackEventsBufferMutex  ,TileSelectedCallbackEventsQueue  ,TileSelectedCallbackEventSubscriptions  ,TileSelectedCallbackEventsSubscriptionsMutex);
	
	DispatchMessages(OnRenderTickCallbackEventsBufferMutex  ,OnRenderTickCallbackEventsQueue  ,OnRenderTickCallbackEventSubscriptions  ,OnRenderTickCallbackEventSubscriptionsMutex);
	DispatchMessages(OnUpdateTickCallbackEventsBufferMutex  ,OnUpdateTickCallbackEventsQueue  ,OnUpdateTickCallbackEventSubscriptions  ,OnUpdateTickCallbackEventSubscriptionsMutex);

	DispatchMessages(OnBeginEventsBufferMutex               ,OnBeginEventsQueue               ,OnBeginEventSubscriptions			   ,OnBeginEventSubscriptionsMutex);
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
