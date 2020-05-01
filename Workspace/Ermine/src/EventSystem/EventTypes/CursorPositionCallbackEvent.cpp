#include "stdafx.h"
#include "CursorPositionCallbackEvent.h"

namespace Ermine
{
	////////////////////////////////////////////////////////
	//CursorPositionCallbackEvent Event Class///////////////
	////////////////////////////////////////////////////////

	Ermine::CursorPositionCallbackEvent::CursorPositionCallbackEvent(int xpos, int ypos)
		:
		xpos(xpos),
		ypos(ypos)
	{}

	std::unique_ptr<CursorPositionCallbackEvent> CursorPositionCallbackEvent::GenerateEvent(int xpos, int ypos)
	{
		std::unique_ptr<CursorPositionCallbackEvent> ptr = std::make_unique<CursorPositionCallbackEvent>(xpos,ypos);
		return std::move(ptr);
	}

	////////////////////////////////////////////////////////
	//Concrete Event Subscription Class/////////////////////
	////////////////////////////////////////////////////////

	CursorPositionCallbackEventSubscription::CursorPositionCallbackEventSubscription(std::function<void(CursorPositionCallbackEvent*)> CallableObject)
		:
		CallableObject(CallableObject)
	{}

	CursorPositionCallbackEventSubscription::CursorPositionCallbackEventSubscription(std::function<void(CursorPositionCallbackEvent*)> CallableObject, std::atomic<bool>& Flag)
		:
		EventSubscription(Flag),
		CallableObject(CallableObject)
	{}

	EventType CursorPositionCallbackEventSubscription::GetEventSubscriptionType()
	{
		return EventType::CursorPositionCallbackEvent;
	}

}
