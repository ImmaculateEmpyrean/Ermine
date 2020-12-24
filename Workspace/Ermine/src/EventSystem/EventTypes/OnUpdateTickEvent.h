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

	class OnUpdateTickEvent :public Event
	{
	public:
		OnUpdateTickEvent() = default;
		OnUpdateTickEvent(float DeltaTime);
		
	private:
		float DeltaTime = 0.0f;
		bool EventHandled = false;

	public:
		virtual EventType GetEventType() override { return EventType::OnUpdateTickEvent; }

		static std::unique_ptr<OnUpdateTickEvent> GenerateEvent(float DeltaTime);

	public:
		virtual bool IsEventHandled() override { return EventHandled; }
		virtual void SetEventHandled() override { EventHandled = true; }

		//Event Specific Implementation
		float GetDeltaTime();
	};

	////////////////////////////////////////////////////////
	//OnRenderTick Event Subscription Class/////////////////
	////////////////////////////////////////////////////////

	class OnUpdateTickEventSubscription :public EventSubscription
	{
	public:
		OnUpdateTickEventSubscription() = delete;
		OnUpdateTickEventSubscription(std::function<void(OnUpdateTickEvent*)> CallableObject, std::atomic<bool>& Flag, std::shared_ptr<Ermine::GeneratedObject> Obj);

	private:
		std::function<void(OnUpdateTickEvent*)> CallableObject;

	public:
		virtual EventType GetEventSubscriptionType() override;


		friend class EventBroadcastStation;
	};
}