#include "stdafx.h"
#include "Actor2DBase.h"

#include "EventSystem/EventBroadcastStation.h"
#include "EventSystem/EventTypes/OnTickEvent.h"
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


	Actor2DBase::Actor2DBase(Actor2DBase& rhs)
		:
		Object()
	{}
	Actor2DBase& Actor2DBase::operator=(Actor2DBase& rhs)
	{
		Object::operator=(rhs);
		return *this;
	}

	Actor2DBase::Actor2DBase(Actor2DBase&& rhs) 
		:
		Object()
	{}
	Actor2DBase& Actor2DBase::operator=(Actor2DBase&& rhs)
	{
		Object::operator=(std::move(rhs));
		return *this;
	}

	Actor2DBase::~Actor2DBase()
	{}

#pragma endregion Constructors
}