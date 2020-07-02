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
		:
		BufferData(BufferData)
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
		BufferData(std::move(rhs.BufferData)),
		vertex_buffer(std::move(rhs.vertex_buffer))
	{
		rhs.vertex_buffer = 0;
	}
	VertexBuffer VertexBuffer::operator=(VertexBuffer&& rhs)
	{
		BufferData = std::move(rhs.BufferData);
		vertex_buffer = std::move(rhs.vertex_buffer);
		rhs.vertex_buffer = 0;

		return *this;
	}
	
	bool VertexBuffer::operator==(VertexBuffer& rhs)
	{
		return BufferData == rhs.BufferData;
	}

	void VertexBuffer::Clear()
	{
		ClearAll();
	}

	void Ermine::VertexBuffer::ClearAll()
	{
		BufferData.clear();
		if (vertex_buffer != 0)
		{
			GLCall(glDeleteBuffers(1, &vertex_buffer));
			vertex_buffer = 0; //0 Means Nothing This Should Not Be Found Hopefully On The Destructor..
		}
	}
	
	void Ermine::VertexBuffer::ClearOpenGLBuffer()
	{
		if (vertex_buffer != 0)
		{
			GLCall(glDeleteBuffers(1, &vertex_buffer));
			vertex_buffer = 0; //0 Means Nothing This Should Not Be Found Hopefully On The Destructor..
		}
	}
	
	void VertexBuffer::GenBufferSubmitDataHelper(unsigned int& buffer, std::vector<float>& Data)
	{
		this->ClearOpenGLBuffer();
		GLCall(glGenBuffers(1,&buffer));
		if(Data.size() > 0)
		{
			GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
			GLCall(glBufferData(GL_ARRAY_BUFFER, Data.size() * sizeof(float), &Data.front(), GL_DYNAMIC_DRAW));
		}
	}
}