#include "stdafx.h"
#include "Vertex.h"

std::pair<float, float> Ermine::Vertex::GetXY()
{
	return std::make_pair(x, y);
}

std::pair<float, float> Ermine::Vertex::GetUV()
{
	return std::make_pair(u, v);
}

int Ermine::Vertex::GetVertexSize()
{
	return sizeof(float) * 4;
}
