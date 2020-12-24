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
		//Empty Vertex Array Can Exist But It Does Need Setup..
		VertexArray();
		VertexArray(std::vector<float>& VertexBuffer, std::vector<uint32_t>& IndexBuffer);

		~VertexArray();
	public:
		VertexArray(const VertexArray& rhs);
		VertexArray operator=(const VertexArray& rhs);

		VertexArray(VertexArray&& rhs);
		VertexArray operator=(VertexArray&& rhs);

		//Generates A Vertex Array
		static std::shared_ptr<VertexArray> Generate();
		static std::shared_ptr<VertexArray> Generate(std::vector<float>& VertexBuffer, std::vector<uint32_t> IndexBuffer);

		void Bind  ();
		void UnBind();

		void SetVertexAttribArray(std::vector<VertexAttribPointerSpecification> SpecContainer);
		std::vector<VertexAttribPointerSpecification> GetVertexAttribArray();

		//Get The Underlying Buffers
		Ermine::VertexBuffer& GetVertexBuffer() { return Vbo; };
		Ermine::IndexBuffer& GetIndexBuffer()   { return Ibo; };

		void PushVertices(std::vector<float> Data);
		void PushIndices(std::vector<uint32_t> Data);

		std::vector<float>& GetVertexData(); //This Function Is Fairly Expensive.. Do Not Use It Often..
		std::vector<uint32_t> GetIndexData(); //This Function Is Fairly Expensive Donot Use It Often..

		void Clear();

		operator Ermine::IndexBuffer()  { return Ibo;			};
		operator Ermine::VertexBuffer() { return Vbo;			};
		operator unsigned int()			{ return vertex_array;  };

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
		Ermine::IndexBuffer  Ibo;

		int AttributesSetCount = 0;
		int NextVertexAttribStartLoc = 0;
		std::vector<VertexAttribPointerSpecification> BufferToStoreAllRecievedSpecification;
	};
}