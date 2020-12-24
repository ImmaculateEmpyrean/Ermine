#pragma once
#include<iostream>
#include<vector>
#include<string>

#include "glm.hpp"

namespace Ermine
{
	struct CameraComponent
	{
		CameraComponent() = default;
		CameraComponent(glm::mat4 Projection)
			:
			Projection(Projection)
		{}

		glm::mat4 Projection = glm::ortho<float>(0.0f, 1920.0f, 1080.0f, 0.0f, -10.0f, 10.0f);
	};
}