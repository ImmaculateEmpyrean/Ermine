#include "stdafx.h"
#include "VertexBuffer.h"

//OpenGL Specific Libraries
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "../OpenGLErrorChecker.h"

namespace Ermine
{
	VertexBuffer::VertexBuffer()
	{
		std::vector<float> EmptyVec;
		GenBufferSubmitDataHelper(vertex_buffer, EmptyVec);
	}
	VertexBuffer::VertexBuffer(std::vector<float>& BufferData)
	{
		GenBufferSubmitDataHelper(vertex_buffer, BufferData);
	}
	VertexBuffer::~VertexBuffer()
	{
		GLCall(glDeleteBuffers(1, &vertex_buffer));
	}


	void VertexBuffer::Bind()
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer));
	}
	void VertexBuffer::UnBind()
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}

	VertexBuffer::VertexBuffer(const VertexBuffer& rhs)
		:
		BufferData(rhs.BufferData)
	{
		GenBufferSubmitDataHelper(vertex_buffer, BufferData);
	}
	VertexBuffer VertexBuffer::operator=(const VertexBuffer& rhs)
	{
		BufferData = rhs.BufferData;
		GenBufferSubmitDataHelper(vertex_buffer, BufferData);

		return *this;
	}

	VertexBuffer::VertexBuffer(VertexBuffer&& rhs)
		:
		BufferData(rhs.BufferData),
		vertex_buffer(rhs.vertex_buffer)
	{
		rhs.vertex_buffer = 0;
	}
	VertexBuffer VertexBuffer::operator=(VertexBuffer&& rhs)
	{
		BufferData = rhs.BufferData;
		vertex_buffer = rhs.vertex_buffer;
		rhs.vertex_buffer = 0;

		return *this;
	}
	
	bool VertexBuffer::operator==(VertexBuffer& rhs)
	{
		return BufferData == rhs.BufferData;
	}
	
	
	void VertexBuffer::GenBufferSubmitDataHelper(unsigned int& buffer, std::vector<float>& Data)
	{
		GLCall(glGenBuffers(1,&buffer));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
		GLCall(glBufferData(GL_ARRAY_BUFFER, Data.size()*sizeof(float), &Data.front(), GL_DYNAMIC_DRAW));
	}
}