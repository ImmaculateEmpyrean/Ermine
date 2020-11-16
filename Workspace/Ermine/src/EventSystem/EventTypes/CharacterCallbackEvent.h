#pragma once
#include "../Event.h"
#include "../EnumEventType.h"

#include<string>
#include<functional>

namespace Ermine
{
	////////////////////////////////////////////////////////
	//CharacterCallbackEvent Event Class////////////////////
	////////////////////////////////////////////////////////

	class CharacterCallbackEvent :public Event
	{
	public:
		CharacterCallbackEvent() = default;
		CharacterCallbackEvent(unsigned int CodePoint);

	private:
		unsigned int CodePoint;
		bool EventHandled = false;

	public:
		virtual EventType GetEventType() override { return EventType::CharacterCallbackEvent; }

		static std::unique_ptr<CharacterCallbackEvent> GenerateEvent(unsigned int CodePoint);

	public:
		virtual bool IsEventHandled() override { return EventHandled; }
		virtual void SetEventHandled() override { EventHandled = true; }

		//Event Specific Implementation
		unsigned int GetCodePoint() { return CodePoint; }
	};

	////////////////////////////////////////////////////////
	//CharacterCallbackEvent Event Subscription Class///////
	////////////////////////////////////////////////////////

	class CharacterCallbackEventSubscription :public EventSubscription
	{
	public:
		CharacterCallbackEventSubscription() = delete;
		CharacterCallbackEventSubscription(std::function<void(CharacterCallbackEvent*)> CallableObject, std::atomic<bool>& Flag,std::shared_ptr<GeneratedObject> Obj);

	private:
		std::function<void(CharacterCallbackEvent*)> CallableObject;

	public:
		virtual EventType GetEventSubscriptionType() override;


		friend class EventBroadcastStation;
	};
}