#pragma once
#include "../Event.h"
#include "../EnumEventType.h"

#include<string>
#include<functional>

namespace Ermine
{
	////////////////////////////////////////////////////////
	//MouseButtonCallback Event Class///////////////////////
	////////////////////////////////////////////////////////

	class OnTickEvent :public Event
	{
	public:
		OnTickEvent() = default;//=delete;
		OnTickEvent(float DeltaTime);

	private:
		float DeltaTime = 0.0f;
		bool EventHandled = false;

	public:
		virtual EventType GetEventType() override { return EventType::OnTickEvent; }

		static std::unique_ptr<OnTickEvent> GenerateEvent(float DeltaTime);

	public:
		virtual bool IsEventHandled() override { return EventHandled; }
		virtual void SetEventHandled() override { EventHandled = true; }

		//Event Specific Implementation
		float GetDeltaTime();
	};

	////////////////////////////////////////////////////////
	//OnTick Event Subscription Class///////////////////////
	////////////////////////////////////////////////////////

	class OnTickEventSubscription :public EventSubscription
	{
	public:
		OnTickEventSubscription() = delete;
		OnTickEventSubscription(std::function<void(OnTickEvent*)> CallableObject, std::atomic<bool>& Flag,std::shared_ptr<Ermine::GeneratedObject> Obj);

	private:
		std::function<void(OnTickEvent*)> CallableObject;

	public:
		virtual EventType GetEventSubscriptionType() override;


		friend class EventBroadcastStation;
	};
}