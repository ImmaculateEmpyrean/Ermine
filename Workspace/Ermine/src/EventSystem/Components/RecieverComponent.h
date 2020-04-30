#pragma once
#include<iostream>
#include<vector>
#include<string>

#include<memory>
#include<functional>
#include<atomic>

#include "../EnumEventType.h"

#include "../EventBroadcastStation.h"

#include "../Event.h"
#include "../EventTypes/TestConcreteEvent.h"

//Here x is the name of the method in THIS class...(Note Context Is Important this uses "this" Operator
#ifndef GenCallableFromMethod(x)
#define GenCallableFromMethod(x) std::bind(x,this,std::placeholders::_1)
#endif 

namespace Ermine
{
	class RecieverComponent
	{
	public:
		//No Point in Having an Empty Reciever
		RecieverComponent() = delete;
		RecieverComponent(std::function<void(Event*)> Callable, std::atomic<bool>& SwitchTOControlIfAnEventCanBeExecuted,
						  EventType SubscriptionType);

	private:
		std::function<void(Event*)> Callable;
		std::atomic<bool>& Switch;
		EventType RefToEventType;

	public:
		//Use This New Function More
		static void Bind(std::function<void(Event*)> Callable, std::atomic<bool>& SwitchTOControlIfAnEventCanBeExecuted,
			EventType SubscriptionType);
	};
}