#pragma once
#include<iostream>
#include<vector>
#include<string>

#include "Physics/Physics.h"
#include "2DPrimitives/Sprite.h"

namespace Ermine
{
	struct RubeLoaderPackage
	{
		b2Vec2 Gravity;

		std::vector<std::shared_ptr<Ermine::PhysicsComponent2D>> Components;
		std::vector<std::shared_ptr<Ermine::JointBase>> Joints;
		std::unordered_map<unsigned int, std::shared_ptr<Ermine::Sprite>> Sprites;

	};
}