#include "stdafx.h"
#include "VertexBase.h"

//TODO- Get Rid Of The Opengl headers here they have no business here
#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace Ermine
{
	VertexBase::VertexBase()
		:
		Position({ 0.0f,0.0f,0.0f }),
		VertexColor({ 1.0f,1.0f,1.0f,1.0f })
	{}
	VertexBase::VertexBase(glm::vec3 PositionData)
		:
		Position(std::move(PositionData)),
		VertexColor({ 1.0f,1.0f,1.0f,1.0f })
	{}
	VertexBase::VertexBase(glm::vec3 PositionData, glm::vec4 VertexColor)
		:
		Position(std::move(PositionData)),
		VertexColor(std::move(VertexColor))
	{}

	VertexBase::VertexBase(std::vector<float> Vertex)
	{
		Position.x = Vertex[0];
		Position.y = Vertex[1];
		Position.z = Vertex[2];

		VertexColor.x = Vertex[3];
		VertexColor.y = Vertex[4];
		VertexColor.z = Vertex[5];
		VertexColor.a = Vertex[6];
	}


	std::vector<std::vector<float>>&& VertexBase::GetVertexBufferSeperated(std::vector<float>& MultipleVertexBuffers)
	{
		std::vector<std::vector<float>> Container;
		int c = 0;
		for (float i : MultipleVertexBuffers)
		{
			if (c == GetNumberOfElementsInVertex())
			{
				c = 0;
				Container.emplace_back(std::vector<float>());
			}

			Container[Container.size() - 1].emplace_back(i);
			c++;
		}
		return std::move(Container);
	}
	std::vector<VertexDataObject> VertexBase::GetVertexData() const
	{
		std::vector<VertexDataObject> Vertex;

		Vertex.emplace_back(Position.x);
		Vertex.emplace_back(Position.y);
		Vertex.emplace_back(Position.z);

		Vertex.emplace_back(VertexColor.x);
		Vertex.emplace_back(VertexColor.y);
		Vertex.emplace_back(VertexColor.z);
		Vertex.emplace_back(VertexColor.a);

		return Vertex;
	}

	Ermine::VertexLayout VertexBase::GetLayout()
	{
		Ermine::VertexLayout Lay;
		Lay.AddSpecification(3, GL_FLOAT);
		Lay.AddSpecification(4, GL_FLOAT);

		return Lay;
	}

	Vertex VertexBase::GetVertex()
	{
		Ermine::Vertex Vert(std::make_shared<Ermine::VertexLayout>(GetLayout()));
		Vert.AppendValue(Position.x);
		Vert.AppendValue(Position.y);
		Vert.AppendValue(Position.z);

		Vert.AppendValue(VertexColor.r);
		Vert.AppendValue(VertexColor.g);
		Vert.AppendValue(VertexColor.b);
		Vert.AppendValue(VertexColor.a);

		return Vert;
	}

	std::vector<uint32_t> VertexBase::GenerateIndexBufferQuad(int NumberOfVerticesToTakeIntoAccount)
	{
		std::vector<uint32_t> IndexBuffer;

		if ((NumberOfVerticesToTakeIntoAccount % 4) != 0)
		{
			STDOUTDefaultLog_Error("Error In The Function GenerateIndexBufferQuad() Inside The Class VertexBase Cpp The NumberOfVerticesToTakenIntoAccount Might Not Be A Multiple Of 4 : The Value Sent IS : ({%d})", NumberOfVerticesToTakeIntoAccount);
		}

		int IndexCounter = 0;
		while (IndexCounter < NumberOfVerticesToTakeIntoAccount)
		{
			IndexBuffer.emplace_back(IndexCounter);
			IndexBuffer.emplace_back(IndexCounter + 1);
			IndexBuffer.emplace_back(IndexCounter + 2);

			IndexBuffer.emplace_back(IndexCounter + 2);
			IndexBuffer.emplace_back(IndexCounter + 3);
			IndexBuffer.emplace_back(IndexCounter);

			IndexCounter = IndexCounter + 4;
		}

		return IndexBuffer;
	}


	VertexBase::operator std::vector<VertexDataObject>() const
	{
		std::vector<Ermine::VertexDataObject> Obj;

		Obj.emplace_back(VertexDataObject(Position.x));
		Obj.emplace_back(VertexDataObject(Position.y));
		Obj.emplace_back(VertexDataObject(Position.z));

		Obj.emplace_back(VertexDataObject(VertexColor.r));
		Obj.emplace_back(VertexDataObject(VertexColor.g));
		Obj.emplace_back(VertexDataObject(VertexColor.b));
		Obj.emplace_back(VertexDataObject(VertexColor.a));

		return Obj;
	}
}