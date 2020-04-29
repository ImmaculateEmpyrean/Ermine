#include "stdafx.h"
#include "BroadcastReciever.h"

BroadcastReciever::BroadcastReciever()
{


	/*static auto station = Ermine::EventBroadcastStation::GetStation();
	static auto funcpointer = std::bind(&BroadcastReciever::Event, this, std::placeholders::_1);
	static Ermine::ConcreteEventSubscription Obj(funcpointer);
	station->QueueSubscription(&Obj);*/
}

void BroadcastReciever::Event(Ermine::Event& eve)
{
	Ermine::ConcreteEvent& conceve = (Ermine::ConcreteEvent&)eve;
	std::cout << "Recieved A Message" << conceve.GetMessageBuffer();
}
