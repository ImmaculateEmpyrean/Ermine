#include "stdafx.h"
#include "VertexBase.h"

#include<glad/glad.h>
#include "Graphics/Renderer/OpenGLErrorChecker.h"

Ermine::VertexBase::VertexBase()
	:
	Position({ 0.0f,0.0f,0.0f }),
	VertexColor({ 0.0f,0.0f,0.0f })
{}
Ermine::VertexBase::VertexBase(glm::vec3 PositionData)
	:
	Position(std::move(PositionData)),
	VertexColor({ 0.0f,0.0f,0.0f })
{}
Ermine::VertexBase::VertexBase(glm::vec3 PositionData, glm::vec3 VertexColor)
	:
	Position(std::move(PositionData)),
	VertexColor(std::move(VertexColor))
{}


std::vector<float> Ermine::VertexBase::GetVertexData()
{
	std::vector<float> Vertex;
	
	Vertex.emplace_back(Position.x);
	Vertex.emplace_back(Position.y);
	Vertex.emplace_back(Position.z);

	Vertex.emplace_back(VertexColor.x);
	Vertex.emplace_back(VertexColor.y);
	Vertex.emplace_back(VertexColor.z);

	return Vertex;
}


void Ermine::VertexBase::SetPositonCoordinates(glm::vec3 Position)
{
	this->Position = Position;
}
glm::vec3 Ermine::VertexBase::GetPositionCoordinates()
{
	return Position;
}


void Ermine::VertexBase::SetVertexColorValue(glm::vec3 VertexColorValue)
{
	VertexColor = VertexColorValue;
}
glm::vec3 Ermine::VertexBase::GetVertexColorValue()
{
	return VertexColor;
}


int Ermine::VertexBase::GetNumberOfElementsInVertex()
{
	return 6;
}

int Ermine::VertexBase::GetVertexSize()
{
	return 6 * sizeof(float);
}

void Ermine::VertexBase::SetVertexAttribArray()
{
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, GetVertexSize(), (void*)0));
	GLCall(glEnableVertexAttribArray(0));

	GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, GetVertexSize(), (void*)(3*sizeof(float))));
	GLCall(glEnableVertexAttribArray(1));
}


std::vector<float> Ermine::VertexBase::operator+(const VertexBase& rhs)
{
	std::vector<float> Vertex;

	//Start Adding First Vertex The One On LHS//

	//Start Adding Position//
	Vertex.emplace_back(Position.x);
	Vertex.emplace_back(Position.y);
	Vertex.emplace_back(Position.z);
	//Ended Adding Position//

	//Start Adding VertexColor//
	Vertex.emplace_back(VertexColor.x);
	Vertex.emplace_back(VertexColor.y);
	Vertex.emplace_back(VertexColor.z);
	//Ended Adding VertexColor//

	//Ended Adding First Vertex The One On LHS//


	//Start Adding Second Vertex The One On RHS//

	//Start Adding Position//
	Vertex.emplace_back(rhs.Position.x);
	Vertex.emplace_back(rhs.Position.y);
	Vertex.emplace_back(rhs.Position.z);
	//Ended Adding Position//

	//Start Adding VertexColor//
	Vertex.emplace_back(rhs.VertexColor.x);
	Vertex.emplace_back(rhs.VertexColor.y);
	Vertex.emplace_back(rhs.VertexColor.z);
	//Ended Adding VertexColor//

	//Ended Adding Second Vertex The One On RHS//

	return Vertex;
}
