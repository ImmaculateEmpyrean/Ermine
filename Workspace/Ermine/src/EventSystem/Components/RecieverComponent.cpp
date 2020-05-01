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
	case EventType::ConcreteEvent: std::unique_ptr<Ermine::ConcreteEventSubscription> Obj = std::make_unique<Ermine::ConcreteEventSubscription>(Callable,Switch);
								   station->QueueSubscription(std::move(Obj));
								   break;
	}
}

void Ermine::RecieverComponent::Bind(std::function<void(Event*)> Callable, std::atomic<bool>& SwitchTOControlIfAnEventCanBeExecuted, EventType SubscriptionType)
{
	auto station = Ermine::EventBroadcastStation::GetStation(); //Gets a Line To The Station
		
	/*switch(SubscriptionType)
	{
	case EventType::ConcreteEvent: Obj = std::dynamic_pointer_cast<Ermine::EventSubscription*>(std::make_unique<Ermine::ConcreteEventSubscription>(Callable, SwitchTOControlIfAnEventCanBeExecuted));
		station->QueueSubscription(std::move(Obj));
		break;
	case EventType::KeyCallbackEvent: Obj = std::make_unique<Ermine::KeyCallbackEventSubscription>(Callable, SwitchTOControlIfAnEventCanBeExecuted);
		station->QueueSubscription(std::move(Obj));
		break;
	case EventType::CharacterCallbackEvent: std::unique_ptr<Ermine::CharacterCallbackEventSubscription> Obj = std::make_unique<Ermine::CharacterCallbackEventSubscription>(Callable, SwitchTOControlIfAnEventCanBeExecuted);
		station->QueueSubscription(std::move(Obj));
		break;
	case EventType::CursorPositionCallbackEvent: std::unique_ptr<Ermine::CursorPositionCallbackEvent> Obj = std::make_unique<Ermine::CursorPositionCallbackEvent>(Callable, SwitchTOControlIfAnEventCanBeExecuted);
		station->QueueSubscription(std::move(Obj));
		break;
	case EventType::MouseButtonCallbackEvent: std::unique_ptr<Ermine::MouseButtonCallbackEvent> Obj = std::make_unique<Ermine::MouseButtonCallbackEvent>(Callable, SwitchTOControlIfAnEventCanBeExecuted);
		station->QueueSubscription(std::move(Obj));
		break;
	case EventType::ScrollCallbackEvent: std::unique_ptr<Ermine::ScrollCallbackEvent> Obj = std::make_unique<Ermine::ScrollCallbackEvent>(Callable, SwitchTOControlIfAnEventCanBeExecuted);
		station->QueueSubscription(std::move(Obj));
		break;
	default: STDOUTDefaultLog_Error("Error This Line Must Never Be Reached.. Got From Reciever::Bind Function");
	}*/

	if (SubscriptionType == EventType::ConcreteEvent)
	{
		std::unique_ptr<Ermine::ConcreteEventSubscription> Obj = std::make_unique<Ermine::ConcreteEventSubscription>(Callable, SwitchTOControlIfAnEventCanBeExecuted);
		station->QueueSubscription(std::move(Obj));
	}
	else if (SubscriptionType == EventType::KeyCallbackEvent)
	{
		std::unique_ptr<Ermine::KeyCallbackEventSubscription> Obj = std::make_unique<Ermine::KeyCallbackEventSubscription>(Callable, SwitchTOControlIfAnEventCanBeExecuted);
		station->QueueSubscription(std::move(Obj));
	}
	else if (SubscriptionType == EventType::CharacterCallbackEvent)
	{
		std::unique_ptr<Ermine::CharacterCallbackEventSubscription> Obj = std::make_unique<Ermine::CharacterCallbackEventSubscription>(Callable, SwitchTOControlIfAnEventCanBeExecuted);
		station->QueueSubscription(std::move(Obj));
	}
	else if (SubscriptionType == EventType::CursorPositionCallbackEvent)
	{
		std::unique_ptr<Ermine::CursorPositionCallbackEventSubscription> Obj = std::make_unique<Ermine::CursorPositionCallbackEventSubscription>(Callable, SwitchTOControlIfAnEventCanBeExecuted);
		station->QueueSubscription(std::move(Obj));
	}
	else if (SubscriptionType == EventType::MouseButtonCallbackEvent)
	{
		std::unique_ptr<Ermine::MouseButtonCallbackEventSubscription> Obj = std::make_unique<Ermine::MouseButtonCallbackEventSubscription>(Callable, SwitchTOControlIfAnEventCanBeExecuted);
		station->QueueSubscription(std::move(Obj));
	}
	else if (SubscriptionType == EventType::ScrollCallbackEvent)
	{
		std::unique_ptr<Ermine::ScrollCallbackEventSubscription> Obj = std::make_unique < Ermine::ScrollCallbackEventSubscription > (Callable, SwitchTOControlIfAnEventCanBeExecuted);
		station->QueueSubscription(std::move(Obj));
	}
	else
	{
		STDOUTDefaultLog_Error("Error This Line Must Never Be Reached.. Got From Reciever::Bind Function");
	}
	
	
}
