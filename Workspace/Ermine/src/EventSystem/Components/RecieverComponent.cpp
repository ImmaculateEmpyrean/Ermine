#include "stdafx.h"
#include "RecieverComponent.h"

#include "../EventTypes/TestConcreteEvent.h"
#include "../EventTypes/KeyCallbackEvent.h"
#include "../EventTypes/CharacterCallbackEvent.h"
#include "../EventTypes/CursorPositionCallbackEvent.h"
#include "../EventTypes/MouseButtonCallbackEvent.h"
#include "../EventTypes/ScrollCallbackEvent.h"
#include "../EventTypes/TileSelectedEvent.h"

Ermine::SubscriptionTicket Ermine::RecieverComponent::Bind(std::function<void(Event*)> Callable, std::atomic<bool>& SwitchTOControlIfAnEventCanBeExecuted, EventType SubscriptionType,std::shared_ptr<Ermine::Object> ErmineObj)
{
	auto station = Ermine::EventBroadcastStation::GetStation(); //Gets a Line To The Station

	if (SubscriptionType == EventType::ConcreteEvent)
	{
		std::unique_ptr<Ermine::ConcreteEventSubscription> Obj =				std::make_unique<Ermine::ConcreteEventSubscription>(Callable, SwitchTOControlIfAnEventCanBeExecuted,ErmineObj);
		return std::move(station->QueueSubscription(std::move(Obj)));
	}
	else if (SubscriptionType == EventType::KeyCallbackEvent)
	{
		std::unique_ptr<Ermine::KeyCallbackEventSubscription> Obj =				std::make_unique<Ermine::KeyCallbackEventSubscription>(Callable, SwitchTOControlIfAnEventCanBeExecuted,ErmineObj);
		return std::move(station->QueueSubscription(std::move(Obj)));
	}
	else if (SubscriptionType == EventType::CharacterCallbackEvent)
	{
		std::unique_ptr<Ermine::CharacterCallbackEventSubscription> Obj =		std::make_unique<Ermine::CharacterCallbackEventSubscription>(Callable, SwitchTOControlIfAnEventCanBeExecuted,ErmineObj);
		return std::move(station->QueueSubscription(std::move(Obj)));
	}
	else if (SubscriptionType == EventType::CursorPositionCallbackEvent)
	{
		std::unique_ptr<Ermine::CursorPositionCallbackEventSubscription> Obj =	std::make_unique<Ermine::CursorPositionCallbackEventSubscription>(Callable, SwitchTOControlIfAnEventCanBeExecuted,ErmineObj);
		return std::move(station->QueueSubscription(std::move(Obj)));
	}
	else if (SubscriptionType == EventType::MouseButtonCallbackEvent)
	{
		std::unique_ptr<Ermine::MouseButtonCallbackEventSubscription> Obj	 =	std::make_unique<Ermine::MouseButtonCallbackEventSubscription>(Callable, SwitchTOControlIfAnEventCanBeExecuted,ErmineObj);
		return std::move(station->QueueSubscription(std::move(Obj)));
	}
	else if (SubscriptionType == EventType::ScrollCallbackEvent)
	{
		std::unique_ptr<Ermine::ScrollCallbackEventSubscription> Obj		 =	std::make_unique < Ermine::ScrollCallbackEventSubscription > (Callable, SwitchTOControlIfAnEventCanBeExecuted,ErmineObj);
		return std::move(station->QueueSubscription(std::move(Obj)));
	}
	else if (SubscriptionType == EventType::TileSelectedEvent)
	{
		std::unique_ptr<Ermine::TileSelectedEventSubscription> Obj			 =	std::make_unique < Ermine::TileSelectedEventSubscription>(Callable, SwitchTOControlIfAnEventCanBeExecuted,ErmineObj);
		return std::move(station->QueueSubscription(std::move(Obj)));
	}
	else if (SubscriptionType == EventType::OnTickEvent)
	{
		std::unique_ptr<Ermine::OnTickEventSubscription> Obj				 =	std::make_unique < Ermine::OnTickEventSubscription>(Callable, SwitchTOControlIfAnEventCanBeExecuted,ErmineObj);
		return std::move(station->QueueSubscription(std::move(Obj)));
	}
	else
	{
		STDOUTDefaultLog_Critical("Error This Line Must Never Be Reached.. Got From Reciever::Bind Function");	
	}
}
