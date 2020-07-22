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


	IndexBuffer IndexBuffer::operator+(IndexBuffer& rhs)
	{
		std::vector<uint32_t> BufferData;

		for (uint32_t i : this->BufferData)
			BufferData.emplace_back(i);

		for (uint32_t i : rhs.BufferData)
			BufferData.emplace_back(i);

		//Construct A New Index Buffer From The Given Data And Return It Back To The USer..
		return IndexBuffer(BufferData);
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
		this->ClearOpenGLBuffer();

		GLCall(glGenBuffers(1, &buffer));

		if (Data.size() > 0)
		{
			GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer));
			GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, Data.size() * sizeof(uint32_t), &Data.front(), GL_DYNAMIC_DRAW));
		}
	}


	void IndexBuffer::Clear()
	{
		ClearAll();
	}
	void IndexBuffer::ClearAll()
	{
		BufferData.clear();
		if (index_buffer != 0)
		{
			GLCall(glDeleteBuffers(1, &index_buffer));
			index_buffer = 0; //0 Means Nothing This Should Not Be Found Hopefully On The Destructor..
		}
	}
	void IndexBuffer::ClearOpenGLBuffer()
	{
		if (index_buffer != 0)
		{
			GLCall(glDeleteBuffers(1, &index_buffer));
			index_buffer = 0; //0 Means Nothing This Should Not Be Found Hopefully On The Destructor..
		}
	}
}