#include "stdafx.h"
#include "TileSelectedEvent.h"

namespace Ermine
{
	////////////////////////////////////////////////////////
	//TileSelected Event Class//////////////////////////////
	////////////////////////////////////////////////////////

	TileSelectedEvent::TileSelectedEvent(std::filesystem::path TilesetPath, int index)
		:
		TilesetPath(TilesetPath),
		index(index)
	{}

	std::unique_ptr<TileSelectedEvent> TileSelectedEvent::GenerateEvent(std::filesystem::path TilesetPath, int index)
	{
		std::unique_ptr<TileSelectedEvent> ptr = std::make_unique<TileSelectedEvent>(TilesetPath,index);
		return std::move(ptr);
	}

	////////////////////////////////////////////////////////
	//TileSelected Event Subscription Class/////////////////
	////////////////////////////////////////////////////////

	TileSelectedEventSubscription::TileSelectedEventSubscription(std::function<void(TileSelectedEvent*)> CallableObject, std::atomic<bool>& Flag,std::shared_ptr<Ermine::GeneratedObject> Obj)
		:
		EventSubscription(Flag,Obj),
		CallableObject(CallableObject)
	{}

	EventType TileSelectedEventSubscription::GetEventSubscriptionType()
	{
		return EventType::TileSelectedEvent;
	}

}
