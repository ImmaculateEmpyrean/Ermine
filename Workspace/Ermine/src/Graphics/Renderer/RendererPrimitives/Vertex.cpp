#include "stdafx.h"
#include "Vertex.h"

namespace Ermine
{
	Vertex::Vertex()
	{}
	Vertex::Vertex(std::shared_ptr<VertexLayout> Layout)
		:
		Layout(Layout)
	{}

	bool Vertex::IsLayoutSame(const Vertex& rhs)
	{
		return Layout == rhs.Layout;
	}

	void Vertex::Set(int Loc, int ElementNumber, VertexDataObject Obj)
	{
		int index = Layout->GetElementIndexInBuffer(Loc, ElementNumber);

		int ElementIndexInBuffer = Layout->GetElementIndexInBuffer(Loc, ElementNumber);
		auto Type = Layout->GetElementType(ElementIndexInBuffer);
		
		if(Type == Obj.GetDataType())
			Buffer[index] = Obj;
		else
		{
			STDOUTDefaultLog_Error("Error : Element To Be Inserted Cannot Be Inserted At The Required Position Due To It Not Conforming With The Layout");
			__debugbreak();
		}
	}
	void Vertex::Erase(int Loc, int EleIndex)
	{
		int index = Layout->GetElementIndexInBuffer(Loc, EleIndex);

		Buffer[index] = VertexDataObject();
	}

	std::pair<std::unique_ptr<std::uint8_t>, std::size_t> Vertex::GetVertex()
	{
		int SizeToBeAllocated = 0;
		SizeToBeAllocated = Buffer.size() * Layout->GetVertexSize();

		std::uint8_t* Pointer;
		Pointer = (std::uint8_t*)std::malloc(SizeToBeAllocated);
		std::memset((void*)Pointer, 0, SizeToBeAllocated);

		int Counter = 0;
		for (auto& Obj : Buffer)
		{
			auto ObjectPair = Obj.GetVertexDataObject();
			std::memcpy(Pointer + Counter, ObjectPair.first, ObjectPair.second);
			Counter = Counter + ObjectPair.second;
		}

		//create a unique pointer to manage the deletion of this buffer..
		std::unique_ptr<uint8_t> UPointer;
		UPointer.reset(Pointer);

		return std::make_pair(UPointer,SizeToBeAllocated);
	}


	void Vertex::Clear()
	{
		int BufferLength = Layout->GetVertexLength();

		Buffer.clear();
		Buffer.resize(BufferLength);
	}

	std::size_t Vertex::GetVertexSize()
	{
		return Layout->GetVertexSize();
	}
}