#include "stdafx.h"
#include "KeyCallbackEvent.h"

namespace Ermine
{
	////////////////////////////////////////////////////////
	//KeyCallback Event Class///////////////////////////////
	////////////////////////////////////////////////////////

	KeyCallbackEvent::KeyCallbackEvent(int key, int scancode, int action, int mods)
		:
		key(key),
		scancode(scancode),
		action(action),
		mods(mods)
	{}

	std::unique_ptr<KeyCallbackEvent> KeyCallbackEvent::GenerateEvent(int key, int scancode, int action, int mods)
	{
		std::unique_ptr<KeyCallbackEvent> ptr = std::make_unique<KeyCallbackEvent>(key,scancode,action,mods);
		return std::move(ptr);
	}

	////////////////////////////////////////////////////////
	//KeyCallback Event Subscription Class//////////////////
	////////////////////////////////////////////////////////

	KeyCallbackEventSubscription::KeyCallbackEventSubscription(std::function<void(KeyCallbackEvent*)> CallableObject, 
															   std::atomic<bool> & Flag,std::shared_ptr<Ermine::GeneratedObject> Obj)
		:
		EventSubscription(Flag,Obj),
		CallableObject(CallableObject)
	{}

	EventType KeyCallbackEventSubscription::GetEventSubscriptionType()
	{
		return EventType::KeyCallbackEvent;
	}
}
