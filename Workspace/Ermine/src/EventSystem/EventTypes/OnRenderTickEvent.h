#pragma once
#include "../Event.h"
#include "../EnumEventType.h"

#include<string>
#include<functional>

namespace Ermine
{
	////////////////////////////////////////////////////////
	//OnRenderTick Event Class//////////////////////////////
	////////////////////////////////////////////////////////

	class OnRenderTickEvent :public Event
	{
	public:
		OnRenderTickEvent() = default;//=delete;
		OnRenderTickEvent(float DeltaTime);

	private:
		float DeltaTime = 0.0f;
		bool EventHandled = false;

	public:
		virtual EventType GetEventType() override { return EventType::OnRenderTickEvent; }

		static std::unique_ptr<OnRenderTickEvent> GenerateEvent(float DeltaTime);

	public:
		virtual bool IsEventHandled() override { return EventHandled; }
		virtual void SetEventHandled() override { EventHandled = true; }

		//Event Specific Implementation
		float GetDeltaTime();
	};

	////////////////////////////////////////////////////////
	//OnRenderTick Event Subscription Class/////////////////
	////////////////////////////////////////////////////////

	class OnRenderTickEventSubscription :public EventSubscription
	{
	public:
		OnRenderTickEventSubscription() = delete;
		OnRenderTickEventSubscription(std::function<void(OnRenderTickEvent*)> CallableObject, std::atomic<bool>& Flag,std::shared_ptr<Ermine::GeneratedObject> Obj);

	private:
		std::function<void(OnRenderTickEvent*)> CallableObject;

	public:
		virtual EventType GetEventSubscriptionType() override;


		friend class EventBroadcastStation;
	};
}