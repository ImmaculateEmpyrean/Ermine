#pragma once
#include <string>
#include "EventSystem/EventBroadcastStation.h"

class Sender
{
public:
	Sender(std::string MessageToBroadcast)
		:
		Message(MessageToBroadcast)
	{}

private:
	std::string Message;

public:
	void Broadcast()
	{
		Ermine::ConcreteEvent* eve = new Ermine::ConcreteEvent(Message);

		auto station = Ermine::EventBroadcastStation::GetStation();
		station->QueueBroadcast(eve);
	}
};