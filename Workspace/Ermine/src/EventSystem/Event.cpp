#include "stdafx.h"
#include "Event.h"

Ermine::EventSubscription::EventSubscription(std::atomic<bool>& CanIRecieveEventNowFlag,std::shared_ptr<Ermine::Object> Obj)
	:
	CanIRecieveEventFlag(CanIRecieveEventNowFlag),
	SubscribedObject(SubscribedObject)
{}