#include "stdafx.h"
#include "Actor2DBase.h"

namespace Ermine
{
	Actor2DBase::~Actor2DBase()
	{
		//This Method Is Not Used As Of Now.. Its Is For The Children Down The Road..
	}


	std::unique_lock<std::mutex>&& Actor2DBase::GetActorStandradMutex()
	{
		return std::move(std::unique_lock<std::mutex>(ActorStandradMutex));
	}

	void Actor2DBase::OnTickActorBase(float DeltaTime)
	{
		std::unique_lock<std::mutex> Loc(ActorStandradMutex); //Lock Releases Automagically When The Function Passes..
		OnTick(DeltaTime);
	}
}