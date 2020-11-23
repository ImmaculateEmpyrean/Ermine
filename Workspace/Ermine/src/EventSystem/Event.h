#pragma once
#include<atomic>
#include "EnumEventType.h"
#include "SubscriptionHealth.h"

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
	class GeneratedObject;

	class EventSubscription //This is an interface and does not actually store data
	{
	public:
		EventSubscription() = delete;
		EventSubscription(std::atomic<bool>& CanIRecieveEventNowFlag,std::shared_ptr<Ermine::GeneratedObject> Obj);

		virtual ~EventSubscription() {}; //This Has To Have a virtual Destructor So That Destructor Calls Are Dispatched Forward..

	public:
		virtual EventType GetEventSubscriptionType() = 0; //There is no point in an event like this.. hence it is a pure virtual method
		std::shared_ptr<GeneratedObject> GetSubscribedObject() { return SubscribedObject; }

		std::atomic<bool>& CanIRecieveEventFlag;

		Ermine::SubscriptionHealth GetSubscriptionHealth() { return Health; }
		void SetSubscriptionHealth(Ermine::SubscriptionHealth SubsHealth) { Health = SubsHealth; }
		void SetSubscriptionTermination() { Health = SubscriptionHealth::WantToTerminate; }

	private:
		std::shared_ptr<Ermine::GeneratedObject> SubscribedObject = nullptr;
		Ermine::SubscriptionHealth Health = Ermine::SubscriptionHealth::Subscribed;
	};
}