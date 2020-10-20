#include "stdafx.h"
#include "Actor2DBase.h"

#include "EventSystem/EventBroadcastStation.h"
#include "EventSystem/EventTypes/OnTickEvent.h"
#include "EventSystem/Components/RecieverComponent.h"

namespace Ermine
{
	Actor2DBase::Actor2DBase(bool OnTickImplemented)
	{
		HelperConstructActorBase(OnTickImplemented);
	}
	Actor2DBase::Actor2DBase(const Actor2DBase& rhs)
	{
		HelperConstructActorBase(rhs.OnTickImplemented);
	}
	Actor2DBase::Actor2DBase(Actor2DBase&& rhs) 
	{
		HelperConstructActorBase(rhs.OnTickImplemented);
	}

	Actor2DBase::~Actor2DBase()
	{
		auto Station = Ermine::EventBroadcastStation::GetStation();
		Station->DestroySubscription(std::move(*OnTickEventTicket));

		//Delete The Allocated Space Even If The Variable Is Moved As Per The Norm..
		delete OnTickEventTicket;
	}


	std::unique_lock<std::recursive_mutex>&& Actor2DBase::GetActorStandradMutex()
	{
		return std::move(std::unique_lock<std::recursive_mutex>(ActorStandradMutex));
	}

	void Actor2DBase::OnTickActorBase(Event* Message)
	{
		std::lock_guard<std::recursive_mutex> Loc(ActorStandradMutex);
		OnTick(((OnTickEvent*)Message)->GetDeltaTime());
	}


#pragma region Helpers
	void Actor2DBase::HelperConstructActorBase(bool RecieveOnTickEvents)
	{
		this->OnTickImplemented = RecieveOnTickEvents;

		if (RecieveOnTickEvents == true)
			OnTickEventTicket = new Ermine::SubscriptionTicket(std::move(Ermine::RecieverComponent::Bind(GenCallableFromMethod(&Actor2DBase::OnTickActorBase), ActorReadyToRecieveEvents, Ermine::EventType::OnTickEvent)));
	}
#pragma endregion Helpers

	void Actor2DBase::OnTick(float DeltaTime)
	{
		return;
	}
}