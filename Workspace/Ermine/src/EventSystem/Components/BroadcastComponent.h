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
		BroadcastComponent();

	private:
		Ermine::EventBroadcastStation* ConnectionToStation;
		std::vector<std::unique_ptr<Event>> EventsThatCanBeBroadcasted;

	public:
		//Recieves A Token Which Can Be USed To instruct The Sender Object To Broadcast Stuff...
		unsigned int AddEventForBroadcasting(std::unique_ptr<Event> EventToQueueForSending);
		void BroadcastEventFromQueue(unsigned int& RecievedToken);

		//Event Will Be Sent To The Station Now...
		void BroadcastEvent(std::unique_ptr<Event> EventToBeBroadcasted);
	};
}
