#pragma once
#include "../Event.h"
#include "../EnumEventType.h"

#include<string>
#include<functional>

namespace Ermine
{
	////////////////////////////////////////////////////////
	//KeyCallback Event Class///////////////////////////////
	////////////////////////////////////////////////////////

	class KeyCallbackEvent :public Event
	{
	public:
		KeyCallbackEvent() = default;//=delete;
		KeyCallbackEvent(int key,int scancode,int action,int mods);

	private:
		
		int key;
		int scancode;
		int action;
		int mods;

		bool EventHandled = false;

	public:
		virtual EventType GetEventType() override { return EventType::KeyCallbackEvent; }

		static std::unique_ptr<KeyCallbackEvent> GenerateEvent(int key, int scancode, int action, int mods);

	public:

		virtual bool IsEventHandled() override { return EventHandled; }
		virtual void SetEventHandled() override { EventHandled = true; }

		//Event Specific Implementation
		int GetKey() { return key; }
		int GetScancode(){ return scancode; }
		int GetAction() { return action; }
		int GetMods() { return mods; }
	};

	////////////////////////////////////////////////////////
	//KeyCallback Event Subscription Class//////////////////
	////////////////////////////////////////////////////////

	class KeyCallbackEventSubscription :public EventSubscription
	{
	public:
		KeyCallbackEventSubscription() = delete;
		KeyCallbackEventSubscription(std::function<void(KeyCallbackEvent*)> CallableObject, std::atomic<bool>& Flag,std::shared_ptr<Ermine::GeneratedObject> Obj);

	private:
		std::function<void(KeyCallbackEvent*)> CallableObject;

	public:
		virtual EventType GetEventSubscriptionType() override;

		friend class EventBroadcastStation;
	};
}