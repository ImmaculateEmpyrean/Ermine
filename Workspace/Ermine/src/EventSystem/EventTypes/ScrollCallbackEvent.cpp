#include "stdafx.h"
#include "ScrollCallbackEvent.h"

////////////////////////////////////////////////////////
//ScrollCallback Event Class////////////////////////////
////////////////////////////////////////////////////////

namespace Ermine
{
	ScrollCallbackEvent::ScrollCallbackEvent(double xoffset, double yoffset)
		:
		xoffset(xoffset),
		yoffset(yoffset)
	{}

	std::unique_ptr<ScrollCallbackEvent> ScrollCallbackEvent::GenerateEvent(double xoffset, double yoffset)
	{
		std::unique_ptr<ScrollCallbackEvent> ptr = std::make_unique<ScrollCallbackEvent>(xoffset,yoffset);
		return std::move(ptr);
	}


////////////////////////////////////////////////////////
//ScrollCallback Event Subscription Class///////////////
////////////////////////////////////////////////////////

	ScrollCallbackEventSubscription::ScrollCallbackEventSubscription(std::function<void(ScrollCallbackEvent*)> CallableObject, std::atomic<bool>& Flag,std::shared_ptr<Ermine::GeneratedObject> Obj)
		:
		EventSubscription(Flag,Obj),
		CallableObject(CallableObject)
	{}

	EventType ScrollCallbackEventSubscription::GetEventSubscriptionType()
	{
		return EventType::ScrollCallbackEvent;
	}

}
