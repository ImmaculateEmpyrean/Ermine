#include "stdafx.h"
#include "VertexBuffer.h"

//OpenGL Specific Libraries
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "../OpenGLErrorChecker.h"

namespace Ermine
{
	VertexBuffer::VertexBuffer(std::shared_ptr<VertexLayout> Layout)
		:
		Layout(Layout)
	{}
	VertexBuffer::VertexBuffer(std::shared_ptr<VertexLayout> Layout,std::vector<float>& BufferData)
		:
		Layout(Layout)
	{
		int VertexSize = Layout->GetVertexLength();
		
		//this assert will probably avoide a lot of black screens..
		assert(BufferData.size() % VertexSize == 0);
		
		for (int i = 0; i < BufferData.size(); i = i + VertexSize)
		{
			Ermine::Vertex Vert(Layout);
			for (int j = 0; j < VertexSize; j++)
				Vert.AppendValue(BufferData[i]);
			AppendVertex(Vert);
		}
	}
	VertexBuffer::~VertexBuffer()
	{
		if (vertex_buffer != 0)
			GLCall(glDeleteBuffers(1, &vertex_buffer));
	}

	void VertexBuffer::Bind()
	{
		if (ReadyState == false)
		{
			//This Function Clears The openGl Buffer If It Does Exist..
			Clear();

			GLCall(glGenBuffers(1, &vertex_buffer));

			if (Buffer.size() > 0)
			{
				auto DataStream = GetDataStream();

				GLCall(glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer));
				GLCall(glBufferData(GL_ARRAY_BUFFER, DataStream.second,(void*)DataStream.first.get(), GL_DYNAMIC_DRAW));
			}
		}
		else
		{
			GLCall(glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer));
		}
	}
	void VertexBuffer::UnBind()
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}


	VertexBuffer::VertexBuffer(const VertexBuffer& rhs)
	{
		operator=(rhs);
	}
	VertexBuffer& VertexBuffer::operator=(const VertexBuffer& rhs)
	{
		ReadyState = false;
		vertex_buffer = 0;

		Buffer = rhs.Buffer;
		Layout = rhs.Layout;
	}

	VertexBuffer::VertexBuffer(VertexBuffer&& rhs)
	{
		operator=(std::move(rhs));
	}
	VertexBuffer& VertexBuffer::operator=(VertexBuffer&& rhs)
	{
		ReadyState = rhs.ReadyState;
		rhs.ReadyState = false;

		vertex_buffer = rhs.vertex_buffer;
		rhs.vertex_buffer = 0;

		Buffer = std::move(rhs.Buffer);
		Layout = std::move(rhs.Layout);
	}

	std::pair<std::unique_ptr<uint8_t>, std::size_t> VertexBuffer::GetDataStream()
	{
		int SizeOfVertexBuffer = 0;
		SizeOfVertexBuffer = Buffer.size() * Layout->GetVertexSize();

		std::uint8_t* Pointer = nullptr;
		Pointer = (std::uint8_t*)new std::uint8_t(SizeOfVertexBuffer);
		std::memset(Pointer, 0, SizeOfVertexBuffer);

		//TODO- this part can be optimized a lot more
		int Counter = 0;
		for (int i = 0; i < Buffer.size(); i++)
		{
			auto Pair = GetDataStream(i);
			std::memcpy(Pointer + Counter, Pair.first.get(), Pair.second);
			Counter = Counter + Pair.second;
		}
		//Ended todo part.

		std::unique_ptr<std::uint8_t> UniquePointer;
		UniquePointer.reset(Pointer);

		return std::make_pair(UniquePointer,SizeOfVertexBuffer);
	}
	std::pair<std::unique_ptr<uint8_t>, std::size_t> VertexBuffer::GetDataStream(int index)
	{
		int SizeOfVertexBuffer = 0;
		SizeOfVertexBuffer = Layout->GetVertexSize();

		std::uint8_t* Pointer = nullptr;
		Pointer = (std::uint8_t*)new std::uint8_t(SizeOfVertexBuffer);
		std::memset(Pointer, 0, SizeOfVertexBuffer);

		Ermine::Vertex Vert = Buffer[index];
		auto V = Vert.GetVertex();
		std::memcpy(Pointer, V.first.get(), V.second);

		std::unique_ptr<std::uint8_t> UniquePointer;
		UniquePointer.reset(Pointer);

		return std::make_pair(UniquePointer, SizeOfVertexBuffer);
	}

	
	VertexBuffer VertexBuffer::Generate(std::shared_ptr<VertexLayout> Layout)
	{
		return std::move(VertexBuffer(Layout));
	}
	VertexBuffer VertexBuffer::Generate(std::shared_ptr<VertexLayout> Layout,std::vector<float> VB)
	{
		return std::move(VertexBuffer(Layout,VB));
	}
	VertexBuffer VertexBuffer::Generate(std::shared_ptr<VertexLayout> Layout, std::vector<Vertex> Vertexes)
	{
		VertexBuffer Vb(Layout);
		for (auto& i : Vertexes)
			Vb.AppendVertex(std::move(i));

		return Vb;
	}
	VertexBuffer VertexBuffer::Generate(std::shared_ptr<VertexLayout> Layout,std::vector<VertexDataObject> VertexBufferData)
	{
		VertexBuffer Vb(Layout);
		int VertexLength = Layout->GetVertexLength();

		if (VertexBufferData.size() % VertexLength != 0)
		{
			STDOUTDefaultLog_Error("VertexBuffer Data Does Not Seem To Correspond Properly With The Layout.. Please Check What Is The Problem..");
			__debugbreak();
		}

		int VertexCounter = 0;
		for (int i = 0; i < VertexBufferData.size(); i = i+ VertexLength)
		{
			for (int j = 0; j < VertexLength; j++)
				Vb.AppendValue(VertexBufferData[j],VertexCounter);
			VertexCounter++; 
		}

		return Vb;
	}


	void VertexBuffer::AddVertex(Vertex Vert, int index)
	{
		ReadyState = false;
		Buffer.insert(Buffer.begin() + index, Vert);
	}
	void VertexBuffer::AppendVertex(Vertex Vert)
	{
		ReadyState = false;
		Buffer.emplace_back(Vert);
	}


	void VertexBuffer::Clear()
	{
		if (vertex_buffer != 0)
		{
			GLCall(glDeleteBuffers(1, &vertex_buffer));
			vertex_buffer = 0;
			ReadyState = false;
		}
	}
	void VertexBuffer::Clear(int BeginIndex, int EndIndex)
	{
		ReadyState = false;
		Buffer.erase(Buffer.begin() + BeginIndex, Buffer.begin() + EndIndex);
	}
}