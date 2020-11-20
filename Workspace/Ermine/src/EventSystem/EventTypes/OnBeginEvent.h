#pragma once
#include "../Event.h"
#include "../EnumEventType.h"

#include<string>
#include<functional>

//This Event Is Generally Only Broadcasted By The Generator Class.. And Only Once.. This Event GEts Desubscribed Once Its Finished Calling..

namespace Ermine
{
	////////////////////////////////////////////////////////
	//OnBeginEvent Event Class//////////////////////////////
	////////////////////////////////////////////////////////

	class OnBeginEvent :public Event
	{
	public:
		OnBeginEvent() = default;
		OnBeginEvent(std::shared_ptr<void> SendPackage);

	private:
		std::shared_ptr<void> Packet = nullptr;
		bool EventHandled = false;

	public:
		virtual EventType GetEventType() override { return EventType::OnBeginEvent; }

		static std::unique_ptr<OnBeginEvent> GenerateEvent(std::shared_ptr<void> Package);

	public:
		virtual bool IsEventHandled() override { return EventHandled; }
		virtual void SetEventHandled() override { EventHandled = true; }

		//Event Specific Implementation
		std::shared_ptr<void> GetHeldPacket();
	};

	////////////////////////////////////////////////////////
	//OnBeginEvent Subscription Class///////////////////////
	////////////////////////////////////////////////////////

	class OnBeginEventSubscription :public EventSubscription
	{
	public:
		OnBeginEventSubscription() = delete;
		OnBeginEventSubscription(std::function<void(OnBeginEvent*)> CallableObject, std::atomic<bool>& Flag, std::shared_ptr<Ermine::GeneratedObject> Obj);

	private:
		std::function<void(OnBeginEvent*)> CallableObject;

	public:
		virtual EventType GetEventSubscriptionType() override;

		friend class EventBroadcastStation;
	};
}