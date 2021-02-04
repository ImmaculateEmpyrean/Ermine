#include "stdafx.h"
#include "IndexBuffer.h"

//OpenGL Specific Libraries
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "../OpenGLErrorChecker.h"

namespace Ermine
{
	IndexBuffer::IndexBuffer()
	{}
	IndexBuffer::IndexBuffer(std::vector<uint32_t>& BufferData)
		:
		BufferData(BufferData)
	{}
	IndexBuffer::~IndexBuffer()
	{
		if(index_buffer != 0)
			GLCall(glDeleteBuffers(1, &index_buffer));
	}


	void IndexBuffer::Bind()
	{
		if (ReadyState == false)
		{
			//This Function Clears The openGl Buffer If It Does Exist..
			Clear();

			GLCall(glGenBuffers(1, &index_buffer));

			if (BufferData.size() > 0)
			{
				GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer));
				GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, BufferData.size() * sizeof(uint32_t), &BufferData.front(), GL_DYNAMIC_DRAW));
			}
		}
		else
		{
			GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer));
		}
	}
	void IndexBuffer::UnBind()
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}


	IndexBuffer::IndexBuffer(const IndexBuffer& rhs)
		:
		BufferData(rhs.BufferData)
	{}
	IndexBuffer IndexBuffer::operator=(const IndexBuffer& rhs)
	{
		BufferData = rhs.BufferData;
		return *this;
	}

	IndexBuffer::IndexBuffer(IndexBuffer&& rhs)
		:
		BufferData(std::move(rhs.BufferData))
	{
		ReadyState = rhs.ReadyState;
		index_buffer = rhs.index_buffer;

		rhs.ReadyState = false;
		rhs.index_buffer = 0;
	}
	IndexBuffer IndexBuffer::operator=(IndexBuffer&& rhs)
	{
		BufferData = std::move(rhs.BufferData);
		
		index_buffer = rhs.index_buffer;
		ReadyState = rhs.ReadyState;

		rhs.index_buffer = 0;
		rhs.ReadyState = false;

		return *this;
	}


	std::vector<uint32_t> IndexBuffer::GetIndices(int StartIndex, int EndIndex)
	{
		std::vector<uint32_t> Buffer;
		std::copy(BufferData.begin() + StartIndex, BufferData.begin() + EndIndex, Buffer.begin());
		return std::move(Buffer);
	}


	void IndexBuffer::AddIndices(std::vector<uint32_t> Indices)
	{
		std::copy(Indices.begin(), Indices.end(), BufferData.end() - 1);
		ReadyState = false;
	}
	void IndexBuffer::AddIndex(uint32_t Index)
	{
		BufferData.emplace_back(Index);
		ReadyState = false;
	}


	void IndexBuffer::EraseIndex(int index)
	{
		BufferData.erase(BufferData.begin() + index);
		ReadyState = false;
	}
	void IndexBuffer::EraseIndices(int BeginIndex, int EndIndex)
	{
		BufferData.erase(BufferData.begin() + BeginIndex, BufferData.begin() + EndIndex);
		ReadyState = false;
	}


	IndexBuffer IndexBuffer::Generate()
	{
		return std::move(IndexBuffer());
	}
	IndexBuffer IndexBuffer::Generate(std::vector<uint32_t> IB)
	{
		return std::move(IndexBuffer(IB));
	}


	void IndexBuffer::Clear()
	{
		if (index_buffer != 0)
		{
			GLCall(glDeleteBuffers(1, &index_buffer));
			index_buffer = 0; 
			ReadyState = false;
		}
	}
}