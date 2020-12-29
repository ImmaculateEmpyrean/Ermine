#pragma once
#include<iostream>
#include<vector>
#include<string>

#include "glm.hpp"

//The Transform Component To Be Used By The Entity Component System..
namespace Ermine
{
	struct TransformComponent
	{
		TransformComponent() = default;
		TransformComponent(glm::mat4 Transform)
			:
			Transform(Transform)
		{}

		glm::mat4 Transform = glm::mat4(1.0f);
	};
}