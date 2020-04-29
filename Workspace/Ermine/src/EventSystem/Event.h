#pragma once
#include "EnumEventType.h"

namespace Ermine
{
	class EventBroadcastStation;

	class Event //This is an interface and does not actually store data
	{
	public:
		Event() = default;
	public:
		virtual EventType GetEventType() = 0; //There is no point in an event like this.. hence it is a pure virtual method
	};


	class EventSubscription //This is an interface and does not actually store data
	{
	public:
		EventSubscription() = default;
	public:
		virtual EventType GetEventSubscriptionType() = 0; //There is no point in an event like this.. hence it is a pure virtual method
		virtual bool IsEventHandled() = 0;
		virtual void SetEventHandled() = 0;
	};
}