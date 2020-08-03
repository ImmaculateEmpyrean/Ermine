#include "stdafx.h"
#include "VertexBase.h"

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

Ermine::VertexBase::VertexBase(std::vector<float> Vertex)
{
	Position.x = Vertex[0];
	Position.y = Vertex[1];
	Position.z = Vertex[2];

	VertexColor.x = Vertex[3];
	VertexColor.y = Vertex[4];
	VertexColor.z = Vertex[5];
}


std::vector<float> Ermine::VertexBase::GetVertexData() const
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

void Ermine::VertexBase::SetVertexAttribArray(VertexArray& Vao)
{
	static std::vector<VertexAttribPointerSpecification> Spec = {
				{3,GL_FLOAT,false},
				{3,GL_FLOAT,false}
	};
	Vao.SetVertexAttribArray(Spec);
}

std::vector<uint32_t> Ermine::VertexBase::GenerateIndexBufferQuad(int NumberOfVerticesToTakeIntoAccount)
{
	std::vector<uint32_t> IndexBuffer;

	if((NumberOfVerticesToTakeIntoAccount % 4) != 0)
	{
		STDOUTDefaultLog_Error("Error In The Function GenerateIndexBufferQuad() Inside The Class VertexBase Cpp The NumberOfVerticesToTakenIntoAccount Might Not Be A Multiple Of 4 : The Value Sent IS : ({%d})", NumberOfVerticesToTakeIntoAccount);
	}

	int IndexCounter = 0;
	while (IndexCounter < NumberOfVerticesToTakeIntoAccount)
	{
		IndexBuffer.emplace_back(IndexCounter);
		IndexBuffer.emplace_back(IndexCounter + 1);
		IndexBuffer.emplace_back(IndexCounter + 3);

		IndexBuffer.emplace_back(IndexCounter + 1);
		IndexBuffer.emplace_back(IndexCounter + 2);
		IndexBuffer.emplace_back(IndexCounter + 3);

		IndexCounter = IndexCounter + 4;
	}

	return IndexBuffer;
}
