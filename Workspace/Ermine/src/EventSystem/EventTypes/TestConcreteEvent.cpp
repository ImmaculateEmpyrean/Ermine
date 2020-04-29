#include "stdafx.h"
#include "TestConcreteEvent.h"

namespace Ermine
{
	////////////////////////////////////////////////////////
	//Concrete Event Class//////////////////////////////////
	////////////////////////////////////////////////////////

	ConcreteEvent::ConcreteEvent(std::string MessageToBeTransmitted)
		:
		MessageBuffer(MessageToBeTransmitted)
	{}

	std::unique_ptr<ConcreteEvent> ConcreteEvent::GenerateEvent(std::string MessageBuffer)
	{
		std::unique_ptr<ConcreteEvent> ptr =std::make_unique<ConcreteEvent>(MessageBuffer);
		return std::move(ptr);
	}

	////////////////////////////////////////////////////////
	//Concrete Event Subscription Class/////////////////////
	////////////////////////////////////////////////////////

	ConcreteEventSubscription::ConcreteEventSubscription(std::function<void(ConcreteEvent*)> CallableObject)
		:
		CallableObject(CallableObject)
	{}

	EventType ConcreteEventSubscription::GetEventSubscriptionType()
	{return EventType::ConcreteEvent;}

}