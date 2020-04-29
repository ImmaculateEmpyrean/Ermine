#include "stdafx.h"
#include "RecieverComponent.h"

Ermine::RecieverComponent::RecieverComponent(std::function<void(Event*)> Callable, std::atomic<bool>& SwitchTOControlIfAnEventCanBeExecuted, EventType SubscriptionType)
	:
	Callable(Callable),
	Switch(SwitchTOControlIfAnEventCanBeExecuted),
	RefToEventType(SubscriptionType)
{
	auto station = Ermine::EventBroadcastStation::GetStation(); //Gets a Line To The Station
	switch (SubscriptionType)
	{
	case EventType::ConcreteEvent: std::unique_ptr<Ermine::ConcreteEventSubscription> Obj = std::make_unique<Ermine::ConcreteEventSubscription>(Callable);
								   station->QueueSubscription(std::move(Obj));
								   break;
	}
}
