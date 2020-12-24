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
		:
		Vbo(VertexBuffer),
		Ibo(IndexBuffer)
	{
		HelperCreateAndBindVertexArray();
	}

	
	VertexArray::~VertexArray()
	{
		GLCall(glDeleteVertexArrays(1, &vertex_array));
	}

	VertexArray::VertexArray(const VertexArray& rhs)
	{
		HelperCopyVertexArray(rhs);
	}

	VertexArray VertexArray::operator=(const VertexArray& rhs)
	{
		HelperCopyVertexArray(rhs);
		return *this;
	}

	VertexArray::VertexArray(VertexArray&& rhs)
	{
		HelperMoveVertexArray(std::move(rhs));
	}

	VertexArray VertexArray::operator=(VertexArray&& rhs)
	{
		HelperMoveVertexArray(std::move(rhs));
		return *this;
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
	
	void VertexArray::SetVertexAttribArray(std::vector<VertexAttribPointerSpecification> SpecContainer)
	{
		//Start This Might Be Wrong So just Delete It If Domething Goes Abnormally Wrong..
		AttributesSetCount = 0;
		NextVertexAttribStartLoc = 0;
		//Ended This Might Be Wrong So just Delete It If Domething Goes Abnormally Wrong..

		//Copy The Buffer Over So That Future Copying And Moving Can Be Done Easily..
		BufferToStoreAllRecievedSpecification = SpecContainer;

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

	std::vector<VertexAttribPointerSpecification> VertexArray::GetVertexAttribArray()
	{
		return BufferToStoreAllRecievedSpecification;
	}

	

	void VertexArray::Clear()
	{
		if (vertex_array != 0)
		{
			GLCall(glDeleteVertexArrays(1, &vertex_array));
		}
	}

	void VertexArray::HelperCopyVertexArray(const VertexArray& rhs)
	{
		HelperCreateAndBindVertexArray();

		std::vector<float> VertexBufferData = rhs.Vbo.GetBufferData();//.Vbo.GetBufferData();
		std::vector<uint32_t> IndexBufferData = rhs.Ibo.GetBufferData();

		Vbo.Clear();
		Ibo.Clear();

		Vbo = Ermine::VertexBuffer(VertexBufferData);
		Ibo = Ermine::IndexBuffer(IndexBufferData);

		BufferToStoreAllRecievedSpecification = rhs.BufferToStoreAllRecievedSpecification;

		SetVertexAttribArray(BufferToStoreAllRecievedSpecification);
	}

	void VertexArray::HelperMoveVertexArray(VertexArray&& rhs)
	{
		vertex_array = rhs.vertex_array;
		rhs.vertex_array = 0;

		Vbo = std::move(rhs.Vbo);
		Ibo = std::move(rhs.Ibo);

		AttributesSetCount = rhs.AttributesSetCount;
		NextVertexAttribStartLoc = rhs.NextVertexAttribStartLoc;
		BufferToStoreAllRecievedSpecification = rhs.BufferToStoreAllRecievedSpecification;
	}

	void VertexArray::HelperCreateAndBindVertexArray()
	{
		Clear();

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

		return 4; //If I Dunno I'll Assume Float
	}
}
