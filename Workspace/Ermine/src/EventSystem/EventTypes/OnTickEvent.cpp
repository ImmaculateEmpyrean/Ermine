#include "stdafx.h"
#include "OnTickEvent.h"

namespace Ermine
{
	////////////////////////////////////////////////////////
	//OnTickCallback Event Class////////////////////////////
	////////////////////////////////////////////////////////
	OnTickEvent::OnTickEvent(float DeltaTime)
		:
		DeltaTime(DeltaTime)
	{}

	std::unique_ptr<OnTickEvent> OnTickEvent::GenerateEvent(float DeltaTime)
	{
		std::unique_ptr<OnTickEvent> ptr = std::make_unique<OnTickEvent>(DeltaTime);
		return std::move(ptr);
	}

	float OnTickEvent::GetDeltaTime()
	{
		return DeltaTime;
	}

	////////////////////////////////////////////////////////
	//OnTickCallback Event Subscription Class///////////////
	////////////////////////////////////////////////////////

	OnTickEventSubscription::OnTickEventSubscription(std::function<void(OnTickEvent*)> CallableObject)
		:
		CallableObject(CallableObject)
	{}

	OnTickEventSubscription::OnTickEventSubscription(std::function<void(OnTickEvent*)> CallableObject, std::atomic<bool>& Flag)
		:
		EventSubscription(Flag),
		CallableObject(CallableObject)
	{}

	EventType OnTickEventSubscription::GetEventSubscriptionType()
	{
		return EventType::OnTickEvent;
	}
}