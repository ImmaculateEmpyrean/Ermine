#include "stdafx.h"
#include "OnBeginEvent.h"

namespace Ermine
{
	////////////////////////////////////////////////////////
	//OnTickCallback Event Class////////////////////////////
	////////////////////////////////////////////////////////
	OnBeginEvent::OnBeginEvent(std::shared_ptr<void> Pack)
		:
		Packet(Pack)
	{}

	std::unique_ptr<OnBeginEvent> OnBeginEvent::GenerateEvent(std::shared_ptr<void> Pack)
	{
		std::unique_ptr<OnBeginEvent> ptr = std::make_unique<OnBeginEvent>(Pack);
		return std::move(ptr);
	}

	std::shared_ptr<void> OnBeginEvent::GetHeldPacket()
	{
		return Packet;
	}

	////////////////////////////////////////////////////////
	//OnTickCallback Event Subscription Class///////////////
	////////////////////////////////////////////////////////

	OnBeginEventSubscription::OnBeginEventSubscription(std::function<void(OnBeginEvent*)> CallableObject, std::atomic<bool>& Flag, std::shared_ptr<Ermine::GeneratedObject> Obj)
		:
		EventSubscription(Flag, Obj),
		CallableObject(CallableObject)
	{}

	EventType OnBeginEventSubscription::GetEventSubscriptionType()
	{
		return EventType::OnBeginEvent;
	}
}
