#include "stdafx.h"
#include "OnUpdateTickEvent.h"

namespace Ermine
{
	////////////////////////////////////////////////////////
	//OnUpdateTick Event Class//////////////////////////////
	////////////////////////////////////////////////////////
	OnUpdateTickEvent::OnUpdateTickEvent(float DeltaTime)
		:
		DeltaTime(DeltaTime)
	{}

	std::unique_ptr<OnUpdateTickEvent> OnUpdateTickEvent::GenerateEvent(float DeltaTime)
	{
		std::unique_ptr<OnUpdateTickEvent> ptr = std::make_unique<OnUpdateTickEvent>(DeltaTime);
		return std::move(ptr);
	}

	float OnUpdateTickEvent::GetDeltaTime()
	{
		return this->DeltaTime;
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