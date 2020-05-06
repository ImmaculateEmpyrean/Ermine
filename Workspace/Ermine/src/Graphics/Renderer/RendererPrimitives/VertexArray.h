#pragma once
#include<iostream>
#include<vector>
#include<string>

#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace Ermine
{
	struct VertexAttribPointerSpecification
	{
		//only specify either 1,2,3,4 
		int NumberOfComponents; 
		//Specify Something Like Gl_FLOAT or something like that
		unsigned int TypeOfTheComponent; 
		//specify if the component is to be normalized.. DEfault false
		bool Normailized = false;
	};

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
		void Bind();
		void UnBind();

		void SetVertexAttribArray(std::vector<VertexAttribPointerSpecification>& SpecContainer);

	public:

	protected:

	protected:

	private:
		void HelperCreateAndBindVertexArray();
		int HelperCalculateSizeOfTheVertex(std::vector<VertexAttribPointerSpecification>& SpecContainer);
		int HelperSizeOfTheComponent(unsigned int Component);

	private:
		unsigned int vertex_array;
		Ermine::VertexBuffer Vbo;
		Ermine::IndexBuffer Ibo;

		int AttributesSetCount = 0;
		int NextVertexAttribStartLoc = 0;
		std::vector<VertexAttribPointerSpecification> BufferToStoreAllRecievedSpecification;


	};
}