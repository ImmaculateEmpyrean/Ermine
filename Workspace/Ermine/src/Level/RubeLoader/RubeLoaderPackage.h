#pragma once
#include<iostream>
#include<vector>
#include<string>

#include<unordered_map>
#include<map>

#include "Physics/Physics.h"
#include "2DPrimitives/Sprite.h"

namespace Ermine
{
	struct RubeLoaderPackage
	{
		b2Vec2 Gravity;

		std::vector<std::shared_ptr<Ermine::PhysicsComponent2D>> Components;
		std::vector<std::shared_ptr<Ermine::JointBase>> Joints;

		//The Unsigned int Is The Body Index..
		std::unordered_map<unsigned int, std::shared_ptr<Ermine::Sprite>> Sprites;

		//The Key Here Is The Render Order While The Value Is The Body Index.. HIGHER THE RENDER ORDER THE LATER THE DRAW..
		std::multimap<unsigned int, unsigned int> RenderOrder;
	};
}