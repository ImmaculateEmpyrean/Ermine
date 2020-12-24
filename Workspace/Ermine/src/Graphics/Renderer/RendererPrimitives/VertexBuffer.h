#pragma once
#include<iostream>
#include<vector>
#include<string>

namespace Ermine
{
	class VertexBuffer
	{
	public:
		VertexBuffer(); //= delete; //No Harm In Having a empty Vertex Buffer But We Must first do some allocation
		VertexBuffer(std::vector<float>& BufferData);

		~VertexBuffer();

	public:
		//This Is Very Fairly Expensive..
		std::vector<float> GetVertexData() const { return BufferData; }
		float GetVertex(int index) { return BufferData[index]; }; //Use This For Reading Instead Of The Operator.. U Wont Be Paying With Reconstruction..
		std::vector<float> GetValues(int StartIndex, int EndIndex);

		//Add Vertexes To The Buffer Using This Method..
		void AddValues(std::vector<float> Vertexes);
		void AddValue(float Val);

		int GetBufferDataLength() { return BufferData.size(); };

		void EraseValue(int index);
		void EraseValue(int BeginIndex, int EndIndex);

		static VertexBuffer Generate();
		static VertexBuffer Generate(std::vector<float> VertexBufferData);

		void Bind();
		void UnBind();

		//These Must Be Overloaded So As To Call Relevant Opengl functions when required..
		VertexBuffer(const VertexBuffer& rhs);
		VertexBuffer operator=(const VertexBuffer& rhs);

		VertexBuffer(VertexBuffer&& rhs);
		VertexBuffer operator=(VertexBuffer&& rhs);

		//Use This Instead Of Calling Bind And All That Ruckus..
		operator unsigned int() { Bind(); return vertex_buffer; };

		//Use This To Easily Access Any Element In BufferData.. Changes Will Also Be Reflected But In The Worst Case Very Expensive.. You Might Be Paying With Reconstruction Just For Reading Data..
		float operator[](unsigned int Index) { ReadyState = false; return BufferData[Index]; };

		void Clear();

	public:

	protected:

	protected:

	private:


	private:
		bool ReadyState = false;

		unsigned int vertex_buffer = 0;
		std::vector<float> BufferData;
	};
}