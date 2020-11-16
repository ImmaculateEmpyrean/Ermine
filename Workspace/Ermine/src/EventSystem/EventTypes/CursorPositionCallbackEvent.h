#pragma once
#include "../Event.h"
#include "../EnumEventType.h"

#include<string>
#include<functional>

namespace Ermine
{
	////////////////////////////////////////////////////////
	//CursorPositionCallbackEvent Event Class///////////////
	////////////////////////////////////////////////////////

	class CursorPositionCallbackEvent :public Event
	{
	public:
		CursorPositionCallbackEvent() = default;
		CursorPositionCallbackEvent(int xpos,int ypos);

	private:
		int xpos;
		int ypos;
		bool EventHandled = false;

	public:
		virtual EventType GetEventType() override { return EventType::CursorPositionCallbackEvent; }

		static std::unique_ptr<CursorPositionCallbackEvent> GenerateEvent(int xpos,int ypos);

	public:
		virtual bool IsEventHandled() override { return EventHandled; }
		virtual void SetEventHandled() override { EventHandled = true; }

		//Event Specific Implementation
		int GetXPos() { return xpos; }
		int GetYPos() { return ypos; }
		std::pair<int, int>GetPosition() { return std::make_pair(xpos, ypos); }
	};

	////////////////////////////////////////////////////////
	//Concrete Event Subscription Class/////////////////////
	////////////////////////////////////////////////////////

	class CursorPositionCallbackEventSubscription :public EventSubscription
	{
	public:
		CursorPositionCallbackEventSubscription() = delete;
		CursorPositionCallbackEventSubscription(std::function<void(CursorPositionCallbackEvent*)> CallableObject, std::atomic<bool>& Flag,std::shared_ptr<Ermine::GeneratedObject> Obj);

	private:
		std::function<void(CursorPositionCallbackEvent*)> CallableObject;

	public:
		virtual EventType GetEventSubscriptionType() override;

		friend class EventBroadcastStation;
	};
}