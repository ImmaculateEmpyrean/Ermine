#include "stdafx.h"
#include "BroadcastComponent.h"

Ermine::EventBroadcastStation* Ermine::BroadcastComponent::ConnectionToStation = nullptr;
std::once_flag Ermine::BroadcastComponent::BroadcastComponentInitializationFlag;

Ermine::BroadcastComponent::BroadcastComponent()
{
	ConnectionToStation = EventBroadcastStation::GetStation();
}

unsigned int Ermine::BroadcastComponent::AddEventForBroadcasting(std::unique_ptr<Event> EventToQueueForSending)
{
	EventsThatCanBeBroadcasted.emplace_back(std::move(EventToQueueForSending));
	return EventsThatCanBeBroadcasted.size() - 1;
}

void Ermine::BroadcastComponent::BroadcastEventFromQueue(unsigned int& RecievedToken)
{
	if (RecievedToken < 0)
	{
		std::cout << "Invalid Token Recieved By BroadcastComponent::BroadcastEventFromQueue returning" << std::endl;
		return;
	}

	BroadcastEvent(std::move(EventsThatCanBeBroadcasted[RecievedToken]));
	EventsThatCanBeBroadcasted.erase(EventsThatCanBeBroadcasted.begin() + RecievedToken);
	RecievedToken = -1;
}

void Ermine::BroadcastComponent::BroadcastEvent(std::unique_ptr<Event> EventToBeBroadcasted)
{
	std::call_once(BroadcastComponentInitializationFlag, []() {
		ConnectionToStation = Ermine::EventBroadcastStation::GetStation();
	});
	
	ConnectionToStation->QueueBroadcast(std::move(EventToBeBroadcasted));
}