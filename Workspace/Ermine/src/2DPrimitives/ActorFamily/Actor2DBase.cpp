#include "stdafx.h"
#include "Actor2DBase.h"

#include "EventSystem/EventBroadcastStation.h"
#include "EventSystem/EventTypes/OnUpdateTickEvent.h"
#include "EventSystem/Components/RecieverComponent.h"

#include "MutexSystem/MutexManager.h"
#include "MutexSystem/MutexGaurd.h"

namespace Ermine
{
#pragma region Constructors
	Actor2DBase::Actor2DBase()
		:
		Object()
	{}

	Actor2DBase::Actor2DBase(std::shared_ptr<Ermine::Material> Mat)
		:
		Object()
	{
		auto Lock = GetObjectMutex();

		//Set The Recieved Material As Default Material..
		ActorMaterial = Mat;
	}


	Actor2DBase::Actor2DBase(Actor2DBase& rhs)
		:
		Object()
	{
		auto ForeignLock = rhs.GetObjectMutex();
		auto Lock = GetObjectMutex();

		ActorMaterial = rhs.ActorMaterial;
	}
	Actor2DBase& Actor2DBase::operator=(Actor2DBase& rhs)
	{
		auto ForeignLock = rhs.GetObjectMutex();
		auto Lock = GetObjectMutex();

		Object::operator=(rhs);
		ActorMaterial = rhs.ActorMaterial;
		return *this;
	}

	Actor2DBase::Actor2DBase(Actor2DBase&& rhs) 
		:
		Object()
	{
		auto ForeignLock = rhs.GetObjectMutex();
		auto Lock = GetObjectMutex();

		ActorMaterial = std::move(rhs.ActorMaterial);
	}
	Actor2DBase& Actor2DBase::operator=(Actor2DBase&& rhs)
	{
		auto ForeignLock = rhs.GetObjectMutex();
		auto lock = GetObjectMutex();

		Object::operator=(std::move(rhs));
		ActorMaterial = std::move(rhs.ActorMaterial);
		return *this;
	}

	Actor2DBase::~Actor2DBase()
	{}

#pragma endregion Constructors


	std::shared_ptr<Ermine::Material> Actor2DBase::GetMaterial()
	{
		auto Lock = Object::GetObjectMutex();
		return ActorMaterial;
	}
	void Actor2DBase::SetMaterial(std::shared_ptr<Ermine::Material> Mat)
	{
		auto Lock = Object::GetObjectMutex();
		ActorMaterial = Mat;
	}
}