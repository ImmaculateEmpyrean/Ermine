#pragma once
#include<iostream>
#include<vector>
#include<string>

namespace Ermine
{
	class VertexBuffer
	{
	public:
		VertexBuffer(); //No Harm In Having a empty Vertex Buffer But We Must first do some allocation
		VertexBuffer(std::vector<float>& BufferData);

		~VertexBuffer();

	public:
		void Bind();
		void UnBind();

		VertexBuffer(const VertexBuffer& rhs);
		VertexBuffer operator=(const VertexBuffer& rhs);

		VertexBuffer(VertexBuffer&& rhs);
		VertexBuffer operator=(VertexBuffer&& rhs);

		//This is Very Expensive Donot USe It...
		bool operator ==(VertexBuffer& rhs);

	public:

	protected:

	protected:

	private:
		void GenBufferSubmitDataHelper(unsigned int& buffer,std::vector<float>& Data);
	private:
		unsigned int vertex_buffer;
		std::vector<float> BufferData;
	};
}