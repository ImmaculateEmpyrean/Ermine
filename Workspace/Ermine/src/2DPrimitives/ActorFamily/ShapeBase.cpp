#include "stdafx.h"
#include "ShapeBase.h"

namespace Ermine
{
#pragma region Constructors
	
	ShapeBase::ShapeBase()
		:
		ImageBase()
	{}

	ShapeBase::ShapeBase(std::vector<Ermine::VertexTextured> Vertexes)
		:
		ImageBase(),
		Vertexes(Vertexes)
	{}

	ShapeBase::ShapeBase(std::vector<std::shared_ptr<Ermine::Sprite>> SpriteBuffer, std::vector<Ermine::VertexTextured> Vertexes)
		:
		ImageBase(SpriteBuffer),
		Vertexes(Vertexes)
	{}

	ShapeBase::~ShapeBase()
	{}
#pragma endregion


#pragma region CopyAndDelete

	ShapeBase::ShapeBase(ShapeBase& rhs)
		:
		ImageBase(rhs)
	{
		auto ForeignLock = GetObjectMutex();
		auto Lock = GetObjectMutex();

		Vertexes = rhs.Vertexes;
		RendererDrawMode = rhs.RendererDrawMode;
	}
	ShapeBase& ShapeBase::operator=(ShapeBase& rhs)		
	{
		auto ForeignLock = GetObjectMutex();
		auto Lock = GetObjectMutex();

		ImageBase::operator=(rhs);

		Vertexes = rhs.Vertexes;
		RendererDrawMode = rhs.RendererDrawMode;

		return *this;
	}
	ShapeBase::ShapeBase(ShapeBase&& rhs)
		:
		ImageBase(std::move(rhs))
	{
		auto ForeignLock = GetObjectMutex();
		auto Lock = GetObjectMutex();

		Vertexes = std::move(rhs.Vertexes);
		RendererDrawMode = std::move(rhs.RendererDrawMode);
	}
	ShapeBase& ShapeBase::operator=(ShapeBase&& rhs)
	{
		auto ForeignLock = GetObjectMutex();
		auto Lock = GetObjectMutex();

		ImageBase::operator=(std::move(rhs));

		Vertexes = std::move(rhs.Vertexes);
		RendererDrawMode = std::move(rhs.RendererDrawMode);

		return *this;
	}
#pragma endregion


#pragma region RenderableGenerationImperatives
	std::vector<float> ShapeBase::GenerateModelSpaceVertexBuffer()
	{
		std::vector<float> VertexBuffer;
		for (Ermine::VertexTextured& i : Vertexes)
			VertexBuffer = VertexBuffer + i;
		
		return VertexBuffer;
	}
	std::vector<Ermine::VertexAttribPointerSpecification> ShapeBase::GetVertexArraySpecification()
	{
		return ImageBase::GetVertexArraySpecification();
	}
	std::vector<uint32_t> ShapeBase::GenerateModelSpaceIndices()
	{
		//Get The Appropriate Lock For Accessing The Data..
		auto Lock = GetObjectMutex();
		
		//This Is The Index Buffer That Is Returned To The User..
		std::vector<uint32_t> BIndexBuffer;

		unsigned NumberOfVertices = Vertexes.size();

		if (NumberOfVertices == 0)
		{
			RendererDrawMode = -1;
			return BIndexBuffer; 
		}
		
		if (NumberOfVertices == 1)
		{
			BIndexBuffer.emplace_back(0);
			RendererDrawMode = GL_POINTS;
			return BIndexBuffer; 
		}

		if (NumberOfVertices == 2)
		{
			BIndexBuffer.emplace_back(0);
			BIndexBuffer.emplace_back(1);

			RendererDrawMode = GL_LINES;

			return BIndexBuffer; //Todo Add Code To Fix Type Of Shape Which Is Line..
		}
			

		//The Number Of Vertices Are Atleast 3
		for (int i = 2; i < NumberOfVertices; i = i + 1) 
		{
			BIndexBuffer.emplace_back(i);
			BIndexBuffer.emplace_back(i - 1);
			BIndexBuffer.emplace_back(0);
		}

		RendererDrawMode = GL_TRIANGLES;

		return BIndexBuffer; //Todo Add Code To Fix Type Of Shape Which Is Triangle..

	}
	int32_t ShapeBase::GetRendererDrawMode()
	{
		//Get The relevant Mutex To Access The Data.
		auto Lock = GetObjectMutex();
		
		return RendererDrawMode;
	}
#pragma endregion


#pragma region QueryObject

	std::vector<Ermine::VertexTextured> ShapeBase::GetAllVertexes()
	{
		auto Lock = GetObjectMutex();
		return Vertexes;
	}
	std::vector<Ermine::VertexTextured> ShapeBase::GetVertexesInRange(int Start, int End)
	{
		auto Lock = GetObjectMutex();

		std::vector<Ermine::VertexTextured> VertexBuffer;
		std::copy(Vertexes.begin() + Start, Vertexes.begin() + End, VertexBuffer.begin());

		return VertexBuffer;
	}
	Ermine::VertexTextured ShapeBase::GetVertex(int Index)
	{
		auto Lock = GetObjectMutex();
		return Vertexes[Index];
	}

	int ShapeBase::GetNumberOfVerticesStored()
	{
		auto Lock = GetObjectMutex();
		return Vertexes.size();
	}

	void ShapeBase::EmplaceBackVertex(Ermine::VertexTextured Vertex)
	{
		//Get The Appropriate Lock For Accessing Data Of The Object..
		auto Lock = GetObjectMutex();

		Vertexes.emplace_back(Vertex);
	}
	void ShapeBase::EmplaceBackVertexes(std::vector<Ermine::VertexTextured> Vertexes)
	{
		//Get The Appropriate Lock For Accessing Data Of The Object..
		auto Lock = GetObjectMutex();

		this->Vertexes.insert(this->Vertexes.end() - 1, Vertexes.begin(), Vertexes.end());
	}
	void ShapeBase::InsertVertex(int index, Ermine::VertexTextured Vertex)
	{
		//Get The Appropriate Lock For Accessing Data Of The Object..
		auto Lock = GetObjectMutex();

		this->Vertexes.insert(Vertexes.begin() + index,Vertex);
	}
	void ShapeBase::InsertVertex(int StartIndex, std::vector<Ermine::VertexTextured> Vertex)
	{
		//Get The Appropriate Lock For Accessing Data Of The Object..
		auto Lock = GetObjectMutex();

		this->Vertexes.insert(Vertexes.begin() + StartIndex, Vertex.begin(),Vertex.end());
	}
#pragma endregion
}