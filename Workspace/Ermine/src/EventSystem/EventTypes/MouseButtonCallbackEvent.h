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

	class MouseButtonCallbackEvent :public Event
	{
	public:
		MouseButtonCallbackEvent() = default;//=delete;
		MouseButtonCallbackEvent(int button,int action,int mods);

	private:
		int button;
		int action;
		int mods;
		bool EventHandled = false;

	public:
		virtual EventType GetEventType() override { return EventType::MouseButtonCallbackEvent; }

		static std::unique_ptr<MouseButtonCallbackEvent> GenerateEvent(int button,int action,int mods);

	public:
		virtual bool IsEventHandled() override { return EventHandled; }
		virtual void SetEventHandled() override { EventHandled = true; }

		//Event Specific Implementation
		int GetButton() { return button; }
		int GetAction() { return action; }
		int GetMods() { return mods; }
	};

	////////////////////////////////////////////////////////
	//MouseButtonCallback Event Subscription Class//////////
	////////////////////////////////////////////////////////

	class MouseButtonCallbackEventSubscription :public EventSubscription
	{
	public:
		MouseButtonCallbackEventSubscription() = delete;
		MouseButtonCallbackEventSubscription(std::function<void(MouseButtonCallbackEvent*)> CallableObject, std::atomic<bool>& Flag,std::shared_ptr<Ermine::GeneratedObject> Obj);

	private:
		std::function<void(MouseButtonCallbackEvent*)> CallableObject;

	public:
		virtual EventType GetEventSubscriptionType() override;


		friend class EventBroadcastStation;
	};
}