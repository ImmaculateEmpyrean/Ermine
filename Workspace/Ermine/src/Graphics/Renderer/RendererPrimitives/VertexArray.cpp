#include "stdafx.h"
#include "VertexArray.h"

#include<cassert>

//only opengl specific
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "../OpenGLErrorChecker.h"

namespace Ermine
{
	VertexArray::VertexArray()
		:
		Vbo(),
		Ibo(),
		Specification()
	{}
	VertexArray::VertexArray(std::vector<float>& VertexBuffer, std::vector<uint32_t>& IndexBuffer)
		:
		Vbo(VertexBuffer),
		Ibo(IndexBuffer),
		Specification()
	{}

	VertexArray::~VertexArray()
	{
		Clear();
	}

	
	//ArrayReady and vertex_array Is Not Copied Intentionally 
	VertexArray::VertexArray(const VertexArray& rhs)
		:
		Vbo(rhs.Vbo),
		Ibo(rhs.Ibo),
		Specification(rhs.Specification)
	{}
	//ArrayReady and vertex_array Is Not Copied Intentionally
	VertexArray& VertexArray::operator=(const VertexArray& rhs)
	{
		Vbo = rhs.Vbo;
		Ibo = rhs.Ibo;
		
		Specification = rhs.Specification;
		return *this;
	}

	VertexArray::VertexArray(VertexArray&& rhs)
		:
		Vbo(std::move(rhs.Vbo)),
		Ibo(std::move(rhs.Ibo)),
		Specification(std::move(rhs.Specification))
	{
		vertex_array = rhs.vertex_array;
		rhs.vertex_array = 0;

		ArrayReady = rhs.ArrayReady;
		rhs.ArrayReady = false;
	}

	VertexArray& VertexArray::operator=(VertexArray&& rhs)
	{
		vertex_array = rhs.vertex_array;
		rhs.vertex_array = 0;

		Vbo = std::move(rhs.Vbo);
		Ibo = std::move(rhs.Ibo);

		ArrayReady = rhs.ArrayReady;

		Specification = std::move(rhs.Specification);

		return *this;
	}

#pragma region Generators
	std::shared_ptr<VertexArray> VertexArray::Generate()
	{
		return std::shared_ptr<VertexArray>(new VertexArray());
	}
	std::shared_ptr<VertexArray> VertexArray::Generate(std::vector<float> VertexBuffer, std::vector<uint32_t> IndexBuffer)
	{
		return std::shared_ptr<VertexArray>(new VertexArray(std::move(VertexBuffer),std::move(IndexBuffer)));
	}
#pragma endregion

	void VertexArray::Bind()
	{
		if (ArrayReady == true)
		{
			GLCall(glBindVertexArray(vertex_array));
			Vbo.Bind();
			Ibo.Bind();
		}
		else
		{
			if(vertex_array != 0)
				GLCall(glDeleteVertexArrays(1, &vertex_array));

			GLCall(glGenVertexArrays(1, &vertex_array));
			GLCall(glBindVertexArray(vertex_array));

			UpdateVertexSpecifications();
		}
	}
	void VertexArray::UnBind()
	{
		GLCall(glBindVertexArray(0));
		Vbo.UnBind();
		Ibo.UnBind();
	}
	
	void VertexArray::SetVertexAttribArray(VertexSpecifics& Specification)
	{
		ArrayReady = false;
		this->Specification = Specification;
	}

	void VertexArray::UpdateVertexSpecifications()
	{
		//Copy The Buffer Over So That Future Copying And Moving Can Be Done Easily..
		this->Specification = Specification;

		//Bind The VertexArray So That We Can Start Setting Up The VertexAttrib Pointers..
		Bind();

		int SizeOfVertexArray = Specification.GetVertexSize();

		unsigned int AttributeCounter = 0;
		for (auto i : Specification.GetSpecifications())
		{
			//Better This Than A Black Screen And Countless Hours Wasted On Debugging X>
			assert(i.StrideUpdated == true);

			GLCall(glVertexAttribPointer(AttributeCounter, i.NumberOfComponents, i.TypeOfTheComponent,
				i.Normailized, SizeOfVertexArray, (void*)i.Stride));

			GLCall(glEnableVertexAttribArray(AttributeCounter++));
		}
	}
	VertexSpecifics VertexArray::GetVertexAttribArray()
	{
		return Specification;
	}


	void VertexArray::PushVertices(std::vector<float> Data)
	{
		//Forward The All To One Layer Deeper
		Vbo.AddValues(std::move(Data));
	}
	void VertexArray::PushIndices(std::vector<uint32_t> Data)
	{
		//Forward The All To One Layer Deeper
		Ibo.AddIndices(std::move(Data));
	}


	std::vector<float> VertexArray::GetVertexData()
	{
		//Ask The Vertex To Provide Me Indices..
		return std::move(Vbo.GetVertexData());
	}
	std::vector<uint32_t> VertexArray::GetIndexData()
	{
		//Ask The IndexBuffer To Provide Me Indices..
		return std::move(Ibo.GetIndexData());
	}


	void VertexArray::Clear()
	{
		if (vertex_array != 0)
			GLCall(glDeleteVertexArrays(1, &vertex_array));
		ArrayReady = false;
	}
}
