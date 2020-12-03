#include "stdafx.h"
#include "ShapeBase.h"

namespace Ermine
{
#pragma region Constructors
	
	ShapeBase::ShapeBase(std::shared_ptr<Ermine::Sprite> Sprite, std::vector<Ermine::VertexTextured> Vertexes)
		:
		ImageBase(Sprite),
		Vertexes(Vertexes)
	{}

	ShapeBase::~ShapeBase()
	{}
#pragma endregion


#pragma region CopyAndDelete

	ShapeBase::ShapeBase(ShapeBase& rhs)		
	{
		auto ForeignLock = GetObjectMutex();
		auto Lock = GetObjectMutex();

		ImageBase::operator=(rhs);
		Vertexes = rhs.Vertexes;
	}
	ShapeBase& ShapeBase::operator=(ShapeBase& rhs)		
	{
		auto ForeignLock = GetObjectMutex();
		auto Lock = GetObjectMutex();

		ImageBase::operator=(rhs);
		Vertexes = rhs.Vertexes;
		return *this;
	}
	ShapeBase::ShapeBase(ShapeBase&& rhs)
	{
		auto ForeignLock = GetObjectMutex();
		auto Lock = GetObjectMutex();

		ImageBase::operator=(std::move(rhs));
		Vertexes = std::move(rhs.Vertexes);
	}
	ShapeBase& ShapeBase::operator=(ShapeBase&& rhs)
	{
		auto ForeignLock = GetObjectMutex();
		auto Lock = GetObjectMutex();

		ImageBase::operator=(std::move(rhs));
		Vertexes = std::move(rhs.Vertexes);
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
	std::vector<uint32_t> ShapeBase::GenerateModelSpaceIndices()
	{
		return std::vector<uint32_t>();
	}
#pragma endregion


#pragma region QueryObject

	std::vector<Ermine::VertexTextured> ShapeBase::GetAllVertexes()
	{
		return std::vector<Ermine::VertexTextured>();
	}
	std::vector<Ermine::VertexTextured> ShapeBase::GetVertexesInRange(int Start, int End)
	{
		return std::vector<Ermine::VertexTextured>();
	}
	Ermine::VertexTextured ShapeBase::GetVertex(int Index)
	{
		return Ermine::VertexTextured();
	}
	void ShapeBase::EmplaceBackVertex(Ermine::VertexTextured Vertex)
	{
	}
	void ShapeBase::EmplaceBackVertexes(std::vector<Ermine::VertexTextured> Vertexes)
	{
	}
	void ShapeBase::InsertVertex(int index, Ermine::VertexTextured Vertex)
	{
	}
	void ShapeBase::InsertVertex(int StartIndex, std::vector<Ermine::VertexTextured> Vertex)
	{
	}
#pragma endregion
}