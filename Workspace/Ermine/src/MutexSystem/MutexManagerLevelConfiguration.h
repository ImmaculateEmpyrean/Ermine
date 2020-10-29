#pragma once
#include<iostream>
#include<vector>
#include<string>

namespace Ermine
{
	enum class MutexLevel : unsigned int
	{
		Empty = 0,

		ActorBase = 1,

		ImageBase = 2,

		Actor2D = 3,
		PhysicsActor = 3

	};
}