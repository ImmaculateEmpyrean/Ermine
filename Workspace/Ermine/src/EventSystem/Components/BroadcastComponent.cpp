#include "stdafx.h"
#include "BroadcastComponent.h"

void Ermine::BroadcastComponent::BroadcastEvent(std::unique_ptr<Event> EventToBeBroadcasted)
{
	Ermine::EventBroadcastStation::GetStation()->QueueBroadcast(std::move(EventToBeBroadcasted));
}

void Ermine::BroadcastComponent::BlockingBroadcast(std::unique_ptr<Event> EventToBeBroadcasted)
{
	Ermine::EventBroadcastStation::GetStation()->BlockingBroadcast(std::move(EventToBeBroadcasted));
}
