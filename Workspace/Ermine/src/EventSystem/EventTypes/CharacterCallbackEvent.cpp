#include "stdafx.h"
#include "CharacterCallbackEvent.h"

namespace Ermine
{
	////////////////////////////////////////////////////////
	//CharacterCallbackEvent Event Class Event Class////////
	////////////////////////////////////////////////////////

	CharacterCallbackEvent::CharacterCallbackEvent(unsigned int CodePoint)
		:
		CodePoint(CodePoint)
	{}

	std::unique_ptr<CharacterCallbackEvent> CharacterCallbackEvent::GenerateEvent(unsigned int CodePoint)
	{
		std::unique_ptr<CharacterCallbackEvent> ptr = std::make_unique<CharacterCallbackEvent>(CodePoint);
		return std::move(ptr);
	}

	////////////////////////////////////////////////////////
	//CharacterCallbackEvent Event Subscription Class///////
	////////////////////////////////////////////////////////

	CharacterCallbackEventSubscription::CharacterCallbackEventSubscription(std::function<void(CharacterCallbackEvent*)> CallableObject, std::atomic<bool>& Flag, std::shared_ptr<Ermine::Object> Obj)
		:
		EventSubscription(Flag,Obj),
		CallableObject(CallableObject)
	{}

	EventType CharacterCallbackEventSubscription::GetEventSubscriptionType()
	{
		return EventType::CharacterCallbackEvent;
	}
}
