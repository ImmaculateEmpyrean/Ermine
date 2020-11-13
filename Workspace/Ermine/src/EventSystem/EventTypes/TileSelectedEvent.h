#pragma once
#include "../Event.h"
#include "../EnumEventType.h"

#include<string>
#include<functional>
#include<filesystem>

namespace Ermine
{
	////////////////////////////////////////////////////////
	//TileSelected Event Class//////////////////////////////
	////////////////////////////////////////////////////////

	class TileSelectedEvent :public Event
	{
	public:
		TileSelectedEvent() = default;
		TileSelectedEvent(std::filesystem::path TilesetPath,int index);

	private:
		std::filesystem::path TilesetPath;
		int index = -1; //Can Transmit Sprite Too Make a Feasability and advantages study to decide in the future..

		bool EventHandled = false;

	public:
		virtual EventType GetEventType() override { return EventType::TileSelectedEvent; }

		static std::unique_ptr<TileSelectedEvent> GenerateEvent(std::filesystem::path TilesetPath, int index);

	public:
		//Event Specific Implementation
		std::filesystem::path GetTilesetPath() { return TilesetPath; }
		int GetIndex() { return index; }

		virtual bool IsEventHandled() override { return EventHandled; }
		virtual void SetEventHandled() override { EventHandled = true; }
	};

	////////////////////////////////////////////////////////
	//TileSelected Event Subscription Class/////////////////
	////////////////////////////////////////////////////////

	class TileSelectedEventSubscription :public EventSubscription
	{
	public:
		TileSelectedEventSubscription() = delete;
		TileSelectedEventSubscription(std::function<void(TileSelectedEvent*)> CallableObject, std::atomic<bool>& Flag,std::shared_ptr<Ermine::Object> Obj);

	private:
		std::function<void(TileSelectedEvent*)> CallableObject;

	public:
		virtual EventType GetEventSubscriptionType() override;


		friend class EventBroadcastStation;
	};
}