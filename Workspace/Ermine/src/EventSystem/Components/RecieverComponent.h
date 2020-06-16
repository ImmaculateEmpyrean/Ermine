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

//Here x is the name of the method in THIS class...(Note Context Is Important this uses "this" Operator
#ifndef GenCallableFromMethod(x)
#define GenCallableFromMethod(x) std::bind(x,this,std::placeholders::_1)
#endif 

namespace Ermine
{
	class RecieverComponent
	{
	public:
		//There Is Absolutely No Reason To Ever Create A Reciever Component..
		RecieverComponent() = delete; 
	
	public:
		//Use This New Function More
		static void Bind(std::function<void(Event*)> Callable, std::atomic<bool>& SwitchTOControlIfAnEventCanBeExecuted,
			EventType SubscriptionType);
	};
}