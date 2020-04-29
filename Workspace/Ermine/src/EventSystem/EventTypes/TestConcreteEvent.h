#pragma once
#include "EventSystem/Event.h"
#include "EventSystem/EnumEventType.h"

#include<string>
#include<functional>

namespace Ermine
{
	////////////////////////////////////////////////////////
	//Concrete Event Class//////////////////////////////////
	////////////////////////////////////////////////////////

	class ConcreteEvent:public Event
	{
	public:
		ConcreteEvent() = default;//=delete;
		ConcreteEvent(std::string MessageToBeTransmitted);

	private:
		std::string MessageBuffer;

	public:
		virtual EventType GetEventType() override { return EventType::ConcreteEvent; }

		static std::unique_ptr<ConcreteEvent> GenerateEvent(std::string MessageBuffer);

	public:
		//Event Specific Implementation
		std::string GetMessageBuffer() { return MessageBuffer; }
	};




	////////////////////////////////////////////////////////
	//Concrete Event Subscription Class/////////////////////
	////////////////////////////////////////////////////////

	class ConcreteEventSubscription:public EventSubscription
	{
	public:
		ConcreteEventSubscription() = delete;
		ConcreteEventSubscription(std::function<void(ConcreteEvent*)> CallableObject);

	private:
		std::function<void(ConcreteEvent*)> CallableObject;
		bool EventHandled = false;

	public:
		virtual EventType GetEventSubscriptionType() override;
		virtual bool IsEventHandled() override { return EventHandled; }
		virtual void SetEventHandled() override { EventHandled = true; }

		friend class EventBroadcastStation;
	};
}