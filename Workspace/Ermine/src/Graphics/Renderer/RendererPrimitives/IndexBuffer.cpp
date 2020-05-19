#include "stdafx.h"
#include "IndexBuffer.h"

//OpenGL Specific Libraries
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "../OpenGLErrorChecker.h"

namespace Ermine
{
	IndexBuffer::IndexBuffer()
	{
		std::vector<uint32_t> EmptyVec;
		GenBufferSubmitDataHelper(index_buffer, EmptyVec);
	}
	IndexBuffer::IndexBuffer(std::vector<uint32_t>& BufferData)
		:
		BufferData(BufferData)
	{
		GenBufferSubmitDataHelper(index_buffer, BufferData);
	}
	IndexBuffer::~IndexBuffer()
	{
		GLCall(glDeleteBuffers(1, &index_buffer));
	}


	void IndexBuffer::Bind()
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer));
	}
	void IndexBuffer::UnBind()
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}

	IndexBuffer::IndexBuffer(const IndexBuffer& rhs)
		:
		BufferData(rhs.BufferData)
	{
		GenBufferSubmitDataHelper(index_buffer, BufferData);
	}
	IndexBuffer IndexBuffer::operator=(const IndexBuffer& rhs)
	{
		BufferData = rhs.BufferData;
		GenBufferSubmitDataHelper(index_buffer, BufferData);

		return *this;
	}

	IndexBuffer::IndexBuffer(IndexBuffer&& rhs)
		:
		BufferData(rhs.BufferData),
		index_buffer(rhs.index_buffer)
	{
		rhs.index_buffer = 0;
	}
	IndexBuffer IndexBuffer::operator=(IndexBuffer&& rhs)
	{
		BufferData = rhs.BufferData;
		index_buffer = rhs.index_buffer;
		rhs.index_buffer = 0;

		return *this;
	}

	bool IndexBuffer::operator==(IndexBuffer& rhs)
	{
		return BufferData == rhs.BufferData;
	}

	int IndexBuffer::GetBufferDataLength()
	{
		return BufferData.size();
	}


	void IndexBuffer::GenBufferSubmitDataHelper(unsigned int& buffer, std::vector<uint32_t>& Data)
	{
		GLCall(glGenBuffers(1, &buffer));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, Data.size()*sizeof(uint32_t), &Data.front(), GL_DYNAMIC_DRAW));
	}
}