#pragma once

#include<iostream>
#include<vector>
#include<string>

namespace Ermine
{
	class IndexBuffer
	{
	public:
		IndexBuffer(); //No Harm In Having a empty Vertex Buffer But We Must first do some allocation
		IndexBuffer(std::vector<uint32_t>& BufferData);

		~IndexBuffer();

	public:
		void Bind();
		void UnBind();

		IndexBuffer(const IndexBuffer& rhs);
		IndexBuffer operator=(const IndexBuffer& rhs);

		IndexBuffer(IndexBuffer&& rhs);
		IndexBuffer operator=(IndexBuffer&& rhs);

		//This is Very Expensive Donot USe It...
		bool operator ==(IndexBuffer& rhs);

	public:

	protected:

	protected:

	private:
		void GenBufferSubmitDataHelper(unsigned int& buffer, std::vector<uint32_t>& Data);
	private:
		unsigned int index_buffer;
		std::vector<uint32_t> BufferData;
	};
}