#pragma once
#include<iostream>
#include<vector>
#include<string>

#include "VertexBuffer.h"
#include "VertexSpecifics.h"

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
		VertexArray& operator=(const VertexArray& rhs);

		VertexArray(VertexArray&& rhs);
		VertexArray& operator=(VertexArray&& rhs);

		//Generates A Vertex Array
		static std::shared_ptr<VertexArray> Generate();
		static std::shared_ptr<VertexArray> Generate(std::vector<float> VertexBuffer, std::vector<uint32_t> IndexBuffer);

		void Bind  ();
		void UnBind();

		void SetVertexAttribArray(VertexSpecifics& Specification);
		VertexSpecifics GetVertexAttribArray();

		//Get The Underlying Buffers
		Ermine::VertexBuffer& GetVertexBuffer() { return Vbo; };
		Ermine::IndexBuffer& GetIndexBuffer()   { return Ibo; };

		//Get Buffer Characteristics..
		std::uint32_t GetIndexBufferLength() { return Ibo.GetBufferDataLength(); }
		std::uint32_t GetVertexBufferLength(){ return Vbo.GetBufferDataLength(); }

		void PushVertices(std::vector<float> Data);
		void PushIndices(std::vector<uint32_t> Data);

		std::vector<float> GetVertexData(); //This Function Is Fairly Expensive.. Do Not Use It Often..
		std::vector<uint32_t> GetIndexData(); //This Function Is Fairly Expensive Donot Use It Often..

		void Clear();

		operator Ermine::IndexBuffer()  { return Ibo;			};
		operator Ermine::VertexBuffer() { return Vbo;			};
		operator unsigned int()			{ return vertex_array;  };

	private:
		//This Is The Backend Function That Sets Up The Vertex Attributes Of The VertexArray..
		void UpdateVertexSpecifications();

	private:
		//OpenGl VertexArray..
		unsigned int vertex_array = 0;
		
		//The Buffers Contained Inside The Vertex Array..
		VertexBuffer Vbo;
		IndexBuffer  Ibo;

		//If The Buffer Is Already.. No Need To Again Construct It For Binding..
		bool ArrayReady = false;

		VertexSpecifics Specification;
	};
}