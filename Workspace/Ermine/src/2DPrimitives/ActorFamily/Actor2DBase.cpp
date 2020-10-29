#include "stdafx.h"
#include "Actor2DBase.h"

#include "EventSystem/EventBroadcastStation.h"
#include "EventSystem/EventTypes/OnTickEvent.h"
#include "EventSystem/Components/RecieverComponent.h"

namespace Ermine
{
#pragma region Constructors
	Actor2DBase::Actor2DBase()
	{
		HelperConstructActorBase();
	}
	Actor2DBase::Actor2DBase(const Actor2DBase& rhs)
	{
		HelperCopyConstructor(rhs);
	}
	Actor2DBase& Actor2DBase::operator=(const Actor2DBase& rhs)
	{
		HelperCopyConstructor(rhs);
		return *this;
	}
	Actor2DBase::Actor2DBase(Actor2DBase&& rhs) 
	{
		HelperMoveConstructor(std::move(rhs));
	}

	Actor2DBase& Actor2DBase::operator=(Actor2DBase&& rhs)
	{
		HelperMoveConstructor(std::move(rhs));
		return *this;
	}

	Actor2DBase::~Actor2DBase()
	{
		auto Station = Ermine::EventBroadcastStation::GetStation();
		Station->DestroySubscription(std::move(*OnTickEventTicket));

		//Delete The Allocated Space Even If The Variable Is Moved As Per The Norm..
		delete OnTickEventTicket;
	}
#pragma endregion Constructors


	void Actor2DBase::OnTick(std::function<void(float)> OnTickFunction)
	{
		this->OnTickFunction = OnTickFunction;
	}

	void Actor2DBase::OnTickActorBase(Event* Message)
	{
		std::lock_guard<std::recursive_mutex> Loc(ActorStandradMutex);

		//First Get The Class Tick Done
		ClassOnTick(((OnTickEvent*)Message)->GetDeltaTime());

		//Get The Specific Event If There Is Any..
		if (OnTickFunction != nullptr)
			OnTickFunction(((OnTickEvent*)Message)->GetDeltaTime());
	}


#pragma region Helpers
	void Actor2DBase::HelperConstructActorBase()
	{
		
		//Subscription Must Be Given Last
		OnTickEventTicket = new Ermine::SubscriptionTicket(std::move(Ermine::RecieverComponent::Bind(GenCallableFromMethod(&Actor2DBase::OnTickActorBase), ActorReadyToRecieveEvents, Ermine::EventType::OnTickEvent)));
	}

	
	void Actor2DBase::HelperCopyConstructor(const Actor2DBase& rhs)
	{
		Object::operator=(rhs);
		OnTickEventTicket = new Ermine::SubscriptionTicket(std::move(Ermine::RecieverComponent::Bind(GenCallableFromMethod(&Actor2DBase::OnTickActorBase), ActorReadyToRecieveEvents, Ermine::EventType::OnTickEvent)));
	}
	
	void Actor2DBase::HelperMoveConstructor(Actor2DBase&& rhs)
	{
		Object::operator=(std::move(rhs));
		OnTickEventTicket = new Ermine::SubscriptionTicket(std::move(Ermine::RecieverComponent::Bind(GenCallableFromMethod(&Actor2DBase::OnTickActorBase), ActorReadyToRecieveEvents, Ermine::EventType::OnTickEvent)));
	}
#pragma endregion Helpers
}