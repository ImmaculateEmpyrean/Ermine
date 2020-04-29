#include "stdafx.h"
#include "Event.h"

Ermine::EventSubscription::EventSubscription()
	:
	CanIRecieveEventFlag(std::atomic<bool>(true)) //Not Recommended To Use This .. Intended For Legacy Purposes...
{}

Ermine::EventSubscription::EventSubscription(std::atomic<bool>& CanIRecieveEventNowFlag)
	:
	CanIRecieveEventFlag(CanIRecieveEventNowFlag) //If Possible Use This Constructor It Surely Is Recommended
{}