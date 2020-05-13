#pragma once
#include<iostream>
#include<vector>
#include<string>

#include<memory>

namespace Ermine
{
	struct Vertex
	{
		float x, y;
		float u, v;

		std::pair<float, float> GetXY();
		std::pair<float, float> GetUV();

		int GetVertexSize();
	};
}