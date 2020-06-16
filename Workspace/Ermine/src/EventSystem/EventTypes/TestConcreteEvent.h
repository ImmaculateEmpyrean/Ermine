#pragma once
#include "../Event.h"
#include "../EnumEventType.h"

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
		ConcreteEvent() = default;
		ConcreteEvent(std::string MessageToBeTransmitted);

	private:
		std::string MessageBuffer;
		bool EventHandled = false;

	public:
		virtual EventType GetEventType() override { return EventType::ConcreteEvent; }

		static std::unique_ptr<ConcreteEvent> GenerateEvent(std::string MessageBuffer);

	public:
		//Event Specific Implementation
		std::string GetMessageBuffer() { return MessageBuffer; }

		virtual bool IsEventHandled() override { return EventHandled; }
		virtual void SetEventHandled() override { EventHandled = true; }
	};

	////////////////////////////////////////////////////////
	//Concrete Event Subscription Class/////////////////////
	////////////////////////////////////////////////////////

	class ConcreteEventSubscription:public EventSubscription
	{
	public:
		ConcreteEventSubscription() = delete;
		ConcreteEventSubscription(std::function<void(ConcreteEvent*)> CallableObject);
		ConcreteEventSubscription(std::function<void(ConcreteEvent*)> CallableObject,std::atomic<bool>& Flag);

	private:
		std::function<void(ConcreteEvent*)> CallableObject;

	public:
		virtual EventType GetEventSubscriptionType() override;
		

		friend class EventBroadcastStation;
	};
}