#pragma once
#include<atomic>
#include "EnumEventType.h"

namespace Ermine
{
	class EventBroadcastStation;

	class Event //This is an interface and does not actually store data
	{
	public:
		Event() = default;
		virtual ~Event() {}; //This Has To Have a virtual Destructor So That Destructor Calls Are Dispatched Forward..

	public:
		virtual EventType GetEventType() = 0; //There is no point in an event like this.. hence it is a pure virtual method

		virtual bool IsEventHandled() = 0;
		virtual void SetEventHandled() = 0;
	};


	class EventSubscription //This is an interface and does not actually store data
	{
	public:
		EventSubscription();
		EventSubscription(std::atomic<bool>& CanIRecieveEventNowFlag);
		virtual ~EventSubscription() {}; //This Has To Have a virtual Destructor So That Destructor Calls Are Dispatched Forward..

	public:
		virtual EventType GetEventSubscriptionType() = 0; //There is no point in an event like this.. hence it is a pure virtual method
		

		std::atomic<bool>& CanIRecieveEventFlag;
	};
}