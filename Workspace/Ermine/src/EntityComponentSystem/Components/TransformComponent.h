#pragma once
#include<iostream>
#include<vector>
#include<string>

#include "glm.hpp"

//The Transform Component To Be Used By The Entity Component System..
struct TransformComponent
{
	TransformComponent() = default;
	TransformComponent(glm::vec3 Transform)
		:
		Transform(Transform)
	{}
	glm::vec3 Transform;
};