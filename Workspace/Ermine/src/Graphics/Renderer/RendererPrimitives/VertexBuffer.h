#pragma once
#include<iostream>
#include<vector>
#include<string>

namespace Ermine
{
	class VertexBuffer
	{
	public:
		VertexBuffer(); //= delete; //No Harm In Having a empty Vertex Buffer But We Must first do some allocation
		VertexBuffer(std::vector<float>& BufferData);

		~VertexBuffer();

	public:
		std::vector<float> GetBufferData() const { return BufferData; }

		void Bind();
		void UnBind();

		VertexBuffer(const VertexBuffer& rhs);
		VertexBuffer operator=(const VertexBuffer& rhs);

		VertexBuffer(VertexBuffer&& rhs);
		VertexBuffer operator=(VertexBuffer&& rhs);

		//This is Very Expensive Donot USe It...
		bool operator ==(VertexBuffer& rhs);

		void Clear();

		VertexBuffer operator +(VertexBuffer& rhs);

	public:

	protected:

	protected:

	private:
		void GenBufferSubmitDataHelper(unsigned int& buffer,std::vector<float>& Data);

		void ClearAll();
		void ClearOpenGLBuffer();

	private:
		unsigned int vertex_buffer = 0;
		std::vector<float> BufferData;
	};
}