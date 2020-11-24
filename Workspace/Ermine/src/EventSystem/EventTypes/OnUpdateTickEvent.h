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
		OnUpdateTickEvent();
		
	private:
		bool EventHandled = false;

	public:
		virtual EventType GetEventType() override { return EventType::OnUpdateTickEvent; }

		static std::unique_ptr<OnUpdateTickEvent> GenerateEvent();

	public:
		virtual bool IsEventHandled() override { return EventHandled; }
		virtual void SetEventHandled() override { EventHandled = true; }
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