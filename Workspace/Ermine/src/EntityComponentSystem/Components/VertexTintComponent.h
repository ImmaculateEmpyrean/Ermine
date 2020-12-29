#pragma once
#include<iostream>
#include<vector>
#include<string>

#include "glm.hpp"

namespace Ermine
{
	struct VertexTintComponent
	{
	public:
		VertexTintComponent()
		{}

		VertexTintComponent(std::vector<glm::vec4> VertexColors)
			:
			VertexColors(VertexColors)
		{}

		std::vector<glm::vec4> VertexColors;
	};
}