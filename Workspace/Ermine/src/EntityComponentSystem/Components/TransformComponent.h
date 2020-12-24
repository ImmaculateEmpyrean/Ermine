#pragma once
#include<iostream>
#include<vector>
#include<string>

#include "glm.hpp"

//The Transform Component To Be Used By The Entity Component System..
struct TransformComponent
{
	TransformComponent() = default;
	TransformComponent(glm::vec4 Transform)
		:
		Transform(Transform)
	{}

	glm::vec4 Transform = glm::vec4(1.0f);
};