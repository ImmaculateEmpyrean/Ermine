#include "stdafx.h"
#include "OnRenderTickEvent.h"

namespace Ermine
{
	////////////////////////////////////////////////////////
	//OnRenderTick Event Class//////////////////////////////
	////////////////////////////////////////////////////////
	OnRenderTickEvent::OnRenderTickEvent(float DeltaTime)
		:
		DeltaTime(DeltaTime)
	{}

	std::unique_ptr<OnRenderTickEvent> OnRenderTickEvent::GenerateEvent(float DeltaTime)
	{
		std::unique_ptr<OnRenderTickEvent> ptr = std::make_unique<OnRenderTickEvent>(DeltaTime);
		return std::move(ptr);
	}

	float OnRenderTickEvent::GetDeltaTime()
	{
		return DeltaTime;
	}

	////////////////////////////////////////////////////////
	//OnRenderTickEvent Subscription Class//////////////////
	////////////////////////////////////////////////////////

	OnRenderTickEventSubscription::OnRenderTickEventSubscription(std::function<void(OnRenderTickEvent*)> CallableObject, std::atomic<bool>& Flag,std::shared_ptr<Ermine::GeneratedObject> Obj)
		:
		EventSubscription(Flag,Obj),
		CallableObject(CallableObject)
	{}

	EventType OnRenderTickEventSubscription::GetEventSubscriptionType()
	{
		return EventType::OnRenderTickEvent;
	}
}