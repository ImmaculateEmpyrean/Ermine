#include "stdafx.h"
#include "VertexArray.h"

//only opengl specific
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "../OpenGLErrorChecker.h"

namespace Ermine
{
	VertexArray::VertexArray()
	{
		HelperCreateAndBindVertexArray();

		Vbo = VertexBuffer(); //Create An Empty Vertex Buffer
		Ibo = IndexBuffer(); //Create An Empty Index Buffer
	}
	VertexArray::VertexArray(std::vector<float>& VertexBuffer, std::vector<uint32_t>& IndexBuffer)
	{
		HelperCreateAndBindVertexArray();

		Vbo = Ermine::VertexBuffer(VertexBuffer); 
		Ibo = Ermine::IndexBuffer(IndexBuffer); 
	}
	VertexArray::VertexArray(Ermine::VertexBuffer& Buffer, Ermine::IndexBuffer& IndexBuffer)
	{
		HelperCreateAndBindVertexArray();
		Vbo = Buffer;
		Ibo = IndexBuffer;
	}
	VertexArray::VertexArray(std::vector<float>& VertexBuffer, Ermine::IndexBuffer& IndexBuffer)
	{
		HelperCreateAndBindVertexArray();
		Vbo = Ermine::VertexBuffer(VertexBuffer);
		Ibo = IndexBuffer;
	}
	VertexArray::VertexArray(Ermine::VertexBuffer& Buffer, std::vector<uint32_t>& IndexBuffer)
	{
		HelperCreateAndBindVertexArray();
		Vbo = Buffer;
		Ibo = Ermine::IndexBuffer(IndexBuffer);
	}
	
	VertexArray::~VertexArray()
	{
		GLCall(glDeleteVertexArrays(1, &vertex_array));
	}


	void VertexArray::Bind()
	{
		GLCall(glBindVertexArray(vertex_array));
		Vbo.Bind();
		Ibo.Bind();
	}
	void VertexArray::UnBind()
	{
		GLCall(glBindVertexArray(0));
		Vbo.UnBind();
		Ibo.UnBind();
	}
	
	void VertexArray::SetVertexAttribArray(std::vector<VertexAttribPointerSpecification>& SpecContainer)
	{
		Bind();
		
		int SizeOfVertexArray = HelperCalculateSizeOfTheVertex(SpecContainer);
		for (auto i : SpecContainer)
		{
			
			GLCall(glVertexAttribPointer(AttributesSetCount, i.NumberOfComponents, i.TypeOfTheComponent,
				i.Normailized, SizeOfVertexArray, (void*)NextVertexAttribStartLoc));

			GLCall(glEnableVertexAttribArray(AttributesSetCount));

			NextVertexAttribStartLoc = NextVertexAttribStartLoc + (i.NumberOfComponents * HelperSizeOfTheComponent(i.TypeOfTheComponent));
			AttributesSetCount++;

			
		}
	}


	void VertexArray::HelperCreateAndBindVertexArray()
	{
		GLCall(glGenVertexArrays(1, &vertex_array));
		GLCall(glBindVertexArray(vertex_array));
	}

	int VertexArray::HelperCalculateSizeOfTheVertex(std::vector<VertexAttribPointerSpecification>& SpecContainer)
	{
		int FinalSize = 0;
		int SizeToAdd = sizeof(float); //Default Because I Was Expecting Only Floats To Arrive As Of Now..
		for (auto i : SpecContainer)
		{			
			FinalSize = FinalSize + (HelperSizeOfTheComponent(i.TypeOfTheComponent) * i.NumberOfComponents);
		}

		return FinalSize;
	}
	int VertexArray::HelperSizeOfTheComponent(unsigned int Component)
	{
		switch (Component)
		{
		case GL_FLOAT: return sizeof(float);
			break;
		default:STDOUTDefaultLog_Error("Problem {} Cannot Handle Anything Other Than GL_FLOAT", __FUNCTION__);
		}

		return 4; //If I Dunno Ill Assume Float
	}
}
