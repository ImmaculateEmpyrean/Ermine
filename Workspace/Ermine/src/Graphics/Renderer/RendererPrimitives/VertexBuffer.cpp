#include "stdafx.h"
#include "VertexBuffer.h"

//OpenGL Specific Libraries
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "../OpenGLErrorChecker.h"

namespace Ermine
{
	VertexBuffer::VertexBuffer()
	{}
	VertexBuffer::VertexBuffer(std::vector<float>& BufferData)
		:
		BufferData(BufferData)
	{}
	VertexBuffer::~VertexBuffer()
	{
		if (vertex_buffer != 0)
			GLCall(glDeleteBuffers(1, &vertex_buffer));
	}


	void VertexBuffer::Bind()
	{
		if (ReadyState == false)
		{
			//This Function Clears The openGl Buffer If It Does Exist..
			Clear();

			GLCall(glGenBuffers(1, &vertex_buffer));

			if (BufferData.size() > 0)
			{
				GLCall(glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer));
				GLCall(glBufferData(GL_ARRAY_BUFFER, BufferData.size() * sizeof(float), &BufferData.front(), GL_DYNAMIC_DRAW));
			}
		}
		else
		{
			GLCall(glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer));
		}
	}
	void VertexBuffer::UnBind()
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}


	VertexBuffer::VertexBuffer(const VertexBuffer& rhs)
		:
		BufferData(rhs.BufferData)
	{}
	VertexBuffer VertexBuffer::operator=(const VertexBuffer& rhs)
	{
		BufferData = rhs.BufferData;
		return *this;
	}

	VertexBuffer::VertexBuffer(VertexBuffer&& rhs)
		:
		BufferData(std::move(rhs.BufferData))
	{
		ReadyState = rhs.ReadyState;
		vertex_buffer = rhs.vertex_buffer;

		rhs.ReadyState = false;
		rhs.vertex_buffer = 0;
	}
	VertexBuffer VertexBuffer::operator=(VertexBuffer&& rhs)
	{
		BufferData = std::move(rhs.BufferData);

		vertex_buffer = rhs.vertex_buffer;
		ReadyState = rhs.ReadyState;

		rhs.vertex_buffer = 0;
		rhs.ReadyState = false;

		return *this;
	}


	std::vector<float> VertexBuffer::GetValues(int StartValue, int EndValue)
	{
		std::vector<float> Buffer;
		std::copy(BufferData.begin() + StartValue, BufferData.begin() + EndValue, Buffer.begin());
		return std::move(Buffer);
	}


	void VertexBuffer::AddValues(std::vector<float> Values)
	{
		std::copy(Values.begin(), Values.end(), BufferData.end() - 1);
		ReadyState = false;
	}
	void VertexBuffer::AddValue(float Value)
	{
		BufferData.emplace_back(Value);
		ReadyState = false;
	}

	int VertexBuffer::GetBufferDataLength()
	{
		return BufferData.size();
	}

	void VertexBuffer::EraseValue(int Value)
	{
		BufferData.erase(BufferData.begin() + Value);
		ReadyState = false;
	}
	void VertexBuffer::EraseValue(int BeginValue, int EndValue)
	{
		BufferData.erase(BufferData.begin() + BeginValue, BufferData.begin() + EndValue);
		ReadyState = false;
	}


	VertexBuffer VertexBuffer::Generate()
	{
		return std::move(VertexBuffer());
	}
	VertexBuffer VertexBuffer::Generate(std::vector<float> VB)
	{
		return std::move(VertexBuffer(VB));
	}


	void VertexBuffer::Clear()
	{
		if (vertex_buffer != 0)
		{
			GLCall(glDeleteBuffers(1, &vertex_buffer));
			vertex_buffer = 0;
			ReadyState = false;
		}
	}
}