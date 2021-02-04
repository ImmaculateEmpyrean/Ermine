#pragma once
#include<iostream>
#include<vector>
#include<string>

#include "VertexBuffer.h"
#include "VertexLayout.h"

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
		static std::shared_ptr<VertexArray> Generate(std::vector<float>			   VertexBuffer, std::vector<uint32_t> IndexBuffer);
		static std::shared_ptr<VertexArray> Generate(std::vector<VertexDataObject> VertexBuffer, std::vector<uint32_t> IndexBuffer);

		void Bind  ();
		void UnBind();

		void SetVertexLayout(VertexLayout& Specification);
		void AddVertexLayoutSpecification(int NumberOfComponents, unsigned int TypeOfComponent, bool Normalized = false);
		void DeleteLayoutSpecification(int index);
		VertexLayout GetVertexLayout();

		//Get The Underlying Buffers
		Ermine::VertexBuffer& GetVertexBuffer() { return Vbo; };
		Ermine::IndexBuffer& GetIndexBuffer()   { return Ibo; };

		//Get Buffer Characteristics..
		std::uint32_t GetIndexBufferLength() { return Ibo.GetBufferLength(); }
		std::uint32_t GetVertexBufferLength(){ return Vbo.GetBufferLength(); }

		void PushVertices(std::vector<float> Data);
		void PushVertices(std::vector<VertexDataObject> Data);

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
		std::unique_ptr<VertexBuffer> Vbo;
		std::unique_ptr<IndexBuffer>  Ibo;

		//If The Buffer Is Already.. No Need To Again Construct It For Binding..
		bool ArrayReady = false;
		std::shared_ptr<VertexLayout> Specification;
	};
}