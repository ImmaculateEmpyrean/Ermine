#pragma once
#include<atomic>
#include "EnumEventType.h"

namespace Ermine
{
	class EventBroadcastStation;

	class Event //This is an interface and does not actually store data
	{
	public:
		Event() = default;
		virtual ~Event() {}; //This Has To Have a virtual Destructor So That Destructor Calls Are Dispatched Forward..

	public:
		virtual EventType GetEventType() = 0; //There is no point in an event like this.. hence it is a pure virtual method

		virtual bool IsEventHandled() = 0;
		virtual void SetEventHandled() = 0;
	};

	//Ermine Object Is Forward Declared Here As We Need To Store A Pointer To It.. In The Proceeding Class..
	class Object;

	class EventSubscription //This is an interface and does not actually store data
	{
	public:
		EventSubscription() = delete;
		EventSubscription(std::atomic<bool>& CanIRecieveEventNowFlag,std::shared_ptr<Ermine::Object> Obj);

		virtual ~EventSubscription() {}; //This Has To Have a virtual Destructor So That Destructor Calls Are Dispatched Forward..

	public:
		virtual EventType GetEventSubscriptionType() = 0; //There is no point in an event like this.. hence it is a pure virtual method
		std::shared_ptr<Object> GetSubscribedObject() { return SubscribedObject; }

		std::atomic<bool>& CanIRecieveEventFlag;

	private:
		std::shared_ptr<Ermine::Object> SubscribedObject = nullptr;
	};
}