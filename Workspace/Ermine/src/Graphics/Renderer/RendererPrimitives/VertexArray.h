#pragma once
#include<iostream>
#include<vector>
#include<string>

#include "VertexAttribPointerSpecification.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace Ermine
{
	class VertexArray
	{
	public:
		VertexArray(); //Empty Vertex Array Can Exist But It Does Need Some Setup..
		VertexArray(std::vector<float>& VertexBuffer, std::vector<uint32_t>& IndexBuffer);
		VertexArray(Ermine::VertexBuffer& Buffer, Ermine::IndexBuffer& IndexBuffer);
		VertexArray(std::vector<float>& VertexBuffer, Ermine::IndexBuffer& IndexBuffer);
		VertexArray(Ermine::VertexBuffer& Buffer, std::vector<uint32_t>& IndexBuffer);

		~VertexArray();

	public:
		VertexArray(const VertexArray& rhs);
		VertexArray operator=(const VertexArray& rhs);

		VertexArray(VertexArray&& rhs);
		VertexArray operator=(VertexArray&& rhs);

		void Bind();
		void UnBind();

		void SetVertexAttribArray(std::vector<VertexAttribPointerSpecification> SpecContainer);
		std::vector<VertexAttribPointerSpecification> GetVertexAttribArray();

		int GetIndexBufferLength();

		void Clear();

		//This Functions Call The Set Vertex Buffer Data Function Inside The Vbo Object..
		//This Function For Some Reason Is Not Working Properly..
		void SetVertexBufferData(std::vector<float> Data);
		//Donot Use This Function Without Good Reason It Is Likely Way Too EXPENSIVE To Break Even..
		std::vector<float> GetVertexBufferData();

		//It Does Add Two Vertex Arrays Together And Returns a new VErtex Array THe VertexBuffers and Index Buffers Are Ahem Added..
		VertexArray operator+(VertexArray& rhs); 

	public:

	protected:

	protected:

	private:
		void HelperCopyVertexArray(const VertexArray& rhs);
		void HelperMoveVertexArray(VertexArray&& rhs);

		void HelperCreateAndBindVertexArray();
		int HelperCalculateSizeOfTheVertex(std::vector<VertexAttribPointerSpecification>& SpecContainer);
		int HelperSizeOfTheComponent(unsigned int Component);

	private:
		unsigned int vertex_array = 0;
		Ermine::VertexBuffer Vbo;
		Ermine::IndexBuffer Ibo;

		int AttributesSetCount = 0;
		int NextVertexAttribStartLoc = 0;
		std::vector<VertexAttribPointerSpecification> BufferToStoreAllRecievedSpecification;


	};
}