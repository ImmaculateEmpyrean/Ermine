#include "stdafx.h"
#include "OnUpdateTickEvent.h"

namespace Ermine
{
	////////////////////////////////////////////////////////
	//OnUpdateTick Event Class//////////////////////////////
	////////////////////////////////////////////////////////
	OnUpdateTickEvent::OnUpdateTickEvent()
	{}

	std::unique_ptr<OnUpdateTickEvent> OnUpdateTickEvent::GenerateEvent()
	{
		std::unique_ptr<OnUpdateTickEvent> ptr = std::make_unique<OnUpdateTickEvent>();
		return std::move(ptr);
	}

	////////////////////////////////////////////////////////
	//OnUpdateTickEvent Subscription Class//////////////////
	////////////////////////////////////////////////////////

	OnUpdateTickEventSubscription::OnUpdateTickEventSubscription(std::function<void(OnUpdateTickEvent*)> CallableObject, std::atomic<bool>& Flag, std::shared_ptr<Ermine::GeneratedObject> Obj)
		:
		EventSubscription(Flag, Obj),
		CallableObject(CallableObject)
	{}

	EventType OnUpdateTickEventSubscription::GetEventSubscriptionType()
	{
		return EventType::OnUpdateTickEvent;
	}
}