#pragma once

#include <iostream>
#include<memory>
#include<vector>

#include "../EventBroadcastStation.h"

#include "../EnumEventType.h"
#include "../Event.h"
#include "../EventTypes/TestConcreteEvent.h"

namespace Ermine
{
	class BroadcastComponent
	{
	public:
		//There Is Absolutely No Reason To Create An Object Of A BroadcastComponent Just Broadcast What Is Needed Using The Api..
		BroadcastComponent() = delete;
		
		static void BroadcastEvent(std::unique_ptr<Event> EventToBeBroadcasted);
		static void BlockingBroadcast(std::unique_ptr<Event> EventToBeBroadcasted); //The Blocking Broadcast Will Instantly Deliever The Evnt To Its Subscribers Using The Same Thread..
	};
}
