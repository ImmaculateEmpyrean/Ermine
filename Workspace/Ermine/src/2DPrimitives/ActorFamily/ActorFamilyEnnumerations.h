#pragma once
#include<iostream>
#include<vector>
#include<string>

namespace Ermine
{
	enum class ActorFamilyIdentifier
	{
		Actor2DBase = 1,

		ImageBase = 2,
		ShapeBase = 3,

		Actor2D = 4,
		PhysicsActor2D = 5,

		MovableShape2D = 6,
		PhysicsShape2D = 7
	};
}