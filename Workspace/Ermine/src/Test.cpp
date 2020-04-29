#include "stdafx.h"

#include "EventSystem/EventBroadcastStation.h"

#include "TestFolder/RecieverWithBroadcastComponent.h"
#include "TestFolder/SenderWithSendComponent.h"

int main()
{
	auto station = Ermine::EventBroadcastStation::GetStation();

	RecieverWithBroadcastComponent RecieverObject2;
	
	//_sleep(500);

	SenderA obj("All Hail The Imperator");
	
	obj.Broadcast();
	
	RecieverWithBroadcastComponent RecieverObject;


	//obj.Broadcast();
	while (true)
	{
		_sleep(1000);
		obj.Broadcast();
	}
	/*_sleep(300);

	BroadcastReciever obj;
	Sender sender("With The Openning of the Transcontinental Telegraph The Country Has long awoken from its slumber, may a future full of smiles and prosperity follow her... -1860");

	sender.Broadcast();

	while (true);*/
}