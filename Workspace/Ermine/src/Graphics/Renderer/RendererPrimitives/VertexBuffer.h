#pragma once
#include<iostream>
#include<vector>
#include<string>

#include "Vertex.h"

namespace Ermine
{
	class VertexBuffer
	{
	public:
		VertexBuffer() = delete; //Cannot Create Vertex Buffer Without Layout...
		VertexBuffer(std::shared_ptr<VertexLayout> Layout);
		VertexBuffer(std::shared_ptr<VertexLayout> Layout,std::vector<float>& BufferData);

		~VertexBuffer();

		//These Must Be Overloaded So As To Call Relevant Opengl functions when required..
		VertexBuffer(const VertexBuffer& rhs);
		VertexBuffer& operator=(const VertexBuffer& rhs);

		VertexBuffer(VertexBuffer&& rhs);
		VertexBuffer& operator=(VertexBuffer&& rhs);

	public:
		//get all the vertexes..
		std::vector<Vertex> GetVertexData() { return Buffer; } //Changes Here Are Not Binding As This Function Essentially Returns You A Copy..
		std::pair<std::unique_ptr<uint8_t>, std::size_t> GetDataStream(); //Changes Here Are Not Binding As This Function Essentially Returns You A Copy..
		std::pair<std::unique_ptr<uint8_t>, std::size_t> GetDataStream(int index); //Changes Here Are Not Binding As This Function Essentially Returns You A Copy..

		//get a singular vertex of choosing..
		Vertex GetVertex(int index) { return Buffer[index]; }; //Changes Here Are Not Binding Since This Function Essentially Returns You A Copy..

		//set vertex data
		void SetVertexData(std::vector<Vertex> Data) { ReadyState = false; Buffer = Data; };
		
		//add a value to one of the vertexes..
		template<typename T> void AddValue(T Value,int VertexNumber, int location = 0, int index = 0)
		{
			if (Buffer.size() < VertexNumber)
				Buffer.resize(VertexNumber + 1, Vertex(Layout));

			int ElementIndex = Layout->GetElementIndexInBuffer(location, index);
			Buffer[VertexNumber].Set(location, index, Ermine::VertexDataObject(T));
			ReadyState = false;
		}
		template<typename T> void AppendValue(T Value,int VertexNumber)
		{
			if (Buffer.size() < VertexNumber)
				Buffer.resize(VertexNumber + 1, Vertex(Layout));

			Buffer[VertexNumber].AppendValue(Value);
			ReadyState = false;
		}
		void AddVertex   (Vertex Vert, int index = 0); //Appends a vertex to the specified position in the buffer.
		void AppendVertex(Vertex Vert); //Appends a vertex to the end of the vertex buffer.

		int GetVertexSize() { return Layout->GetVertexSize(); };
		int BufferSize() { return Layout->GetVertexSize() * Buffer.size(); };

		int GetVertexLength() { return Layout->GetVertexLength(); };
		int GetBufferLength() { return Buffer.size() * Layout->GetVertexLength(); };


		static VertexBuffer Generate(std::shared_ptr<VertexLayout> Layout);
		static VertexBuffer Generate(std::shared_ptr<VertexLayout> Layout, std::vector<float> VertexBufferData);
		static VertexBuffer Generate(std::shared_ptr<VertexLayout> Layout, std::vector<Vertex> VertexBufferData);
		static VertexBuffer Generate(std::shared_ptr<VertexLayout> Layout, std::vector<VertexDataObject> VertexBufferData);

		void Bind();
		void UnBind();

		//Use This Instead Of Calling Bind And All That Ruckus..
		operator unsigned int() { Bind(); return vertex_buffer; };

		//Use This To Easily Access Any Element In BufferData.. Changes Will Also Be Reflected But In The Worst Case Very Expensive.. You Might Be Paying With Reconstruction Just For Reading Data..
		Vertex& operator[](unsigned int Index) { ReadyState = false; return Buffer[Index]; };

		//Clears The Entire VertexBuffer..
		void Clear();
		void Clear(int BeginIndex, int EndIndex); //
	public:

	protected:

	protected:

	private:


	private:
		bool ReadyState = false;
		unsigned int vertex_buffer = 0;

		std::vector<Vertex> Buffer;
		std::shared_ptr<VertexLayout> Layout;
	};
}