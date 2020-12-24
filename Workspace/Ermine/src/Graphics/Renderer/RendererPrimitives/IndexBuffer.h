#pragma once

#include<iostream>
#include<vector>
#include<string>

namespace Ermine
{
	class IndexBuffer
	{
	public:
		IndexBuffer(); //= delete; //No Harm In Having a empty Vertex Buffer But We Must first do some allocation
		IndexBuffer(std::vector<uint32_t>& BufferData);

		~IndexBuffer();

	public:
		//This Is Very Fairly Expensive..
		std::vector<uint32_t> GetIndexData() const { return BufferData; }
		uint32_t Getindex(int index) { return BufferData[index]; }; //Use This For Reading Instead Of The Operator.. U Wont Be Paying With Reconstruction..
		std::vector<uint32_t> GetIndices(int StartIndex, int EndIndex);

		//Add Indices To The Buffer Using This Method..
		void AddIndices(std::vector<uint32_t> Indices);
		void AddIndex(uint32_t Index);

		int GetBufferDataLength();

		void EraseIndex(int index);
		void EraseIndices(int BeginIndex, int EndIndex);

		static IndexBuffer Generate();
		static IndexBuffer Generate(std::vector<uint32_t> IndexBufferData);

		void Bind();
		void UnBind();

		//These Must Be Overloaded So As To Call Relevant Opengl functions when required..
		IndexBuffer(const IndexBuffer& rhs);
		IndexBuffer operator=(const IndexBuffer& rhs);

		IndexBuffer(IndexBuffer&& rhs);
		IndexBuffer operator=(IndexBuffer&& rhs);

		//Use This Instead Of Calling Bind And All That Ruckus..
		operator unsigned int() { Bind(); return index_buffer; };

		//Use This To Easily Access Any Element In BufferData.. Changes Will Also Be Reflected But In The Worst Case Very Expensive.. You Might Be Paying With Reconstruction Just For Reading Data..
		uint32_t operator[](unsigned int Index) { ReadyState = false; return BufferData[Index]; };
		
		void Clear();

	public:

	protected:

	protected:

	private:
		

	private:
		bool ReadyState = false;

		unsigned int index_buffer = 0;
		std::vector<uint32_t> BufferData;
	};
}