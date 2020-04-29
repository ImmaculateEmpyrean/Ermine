#include "stdafx.h"

#include "EventSystem/EventBroadcastStation.h"

#include "TestFolder/RecieverWithBroadcastComponent.h"
#include "TestFolder/SenderWithSendComponent.h"

int main()
{
	auto station = Ermine::EventBroadcastStation::GetStation(); //This also tends to initialize the class

	RecieverWithBroadcastComponent RecieverObject2;

	SenderA obj("All Hail The Imperator");
	
	obj.Broadcast();
	
	RecieverWithBroadcastComponent RecieverObject;

	while (true)
	{
		_sleep(1000);
		obj.Broadcast();
	}
}