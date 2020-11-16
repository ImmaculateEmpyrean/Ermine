#pragma once
#include "../Event.h"
#include "../EnumEventType.h"

#include<string>
#include<functional>

namespace Ermine
{
	////////////////////////////////////////////////////////
	//ScrollCallback Event Class////////////////////////////
	////////////////////////////////////////////////////////

	class ScrollCallbackEvent :public Event
	{
	public:
		ScrollCallbackEvent() = default;//=delete;
		ScrollCallbackEvent(double xoffset,double yoffset);

	private:
		double xoffset;
		double yoffset;
		bool EventHandled = false;

	public:
		virtual EventType GetEventType() override { return EventType::ScrollCallbackEvent; }

		static std::unique_ptr<ScrollCallbackEvent>GenerateEvent(double xoffset,double yoffset);

	public:
		//Event Specific Implementation
		double GetXoffset() { return xoffset; }
		double GetYoffset() { return yoffset; }
		std::pair<double, double> GetOffset() { return std::make_pair(xoffset, yoffset); }

		virtual bool IsEventHandled() override { return EventHandled; }
		virtual void SetEventHandled() override { EventHandled = true; }
	};

	////////////////////////////////////////////////////////
	//ScrollCallback Event Subscription Class///////////////
	////////////////////////////////////////////////////////

	class ScrollCallbackEventSubscription :public EventSubscription
	{
	public:
		ScrollCallbackEventSubscription() = delete;
		ScrollCallbackEventSubscription(std::function<void(ScrollCallbackEvent*)> CallableObject, std::atomic<bool>& Flag,std::shared_ptr<Ermine::GeneratedObject> Obj);

	private:
		std::function<void(ScrollCallbackEvent*)> CallableObject;

	public:
		virtual EventType GetEventSubscriptionType() override;


		friend class EventBroadcastStation;
	};
}