#include "stdafx.h"
#include "VertexLayout.h"

#include<unordered_map>

//TODO: Get Rid Of These Two Headers.. They Have Absolutely No Business Here..
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include "Graphics/Renderer/OpenGLErrorChecker.h"

namespace Ermine
{
	static std::size_t GetSizeOfTheComponentType(unsigned int ComponentType)
	{
		//TODO: Update The Unordered Map In The Future With Syuff Like Unsigned int and stuff like that..
		switch(ComponentType)
		{
		case GL_FLOAT: return sizeof(float);
			break;
		default:STDOUTDefaultLog_Error("Error in {1}, Cannot Find The Size Of Our ComponentType", __FUNCTION__);
		}

		return 4; //If I Dunno I'll Assume Float
	}
	

	void VertexLayout::UpdateSpecification()
	{
		int SizeOfVertexArray = GetVertexSize();

		unsigned int AttributeCounter = 0;
		for (auto i : Specs)
		{
			//Better This Than A Black Screen And Countless Hours Wasted On Debugging X>
			assert(i.StrideUpdated == true);

			GLCall(glVertexAttribPointer(AttributeCounter, i.NumberOfComponents, i.TypeOfTheComponent,
				i.Normailized, SizeOfVertexArray, (void*)i.Stride));

			GLCall(glEnableVertexAttribArray(AttributeCounter++));
		}
	}

	std::size_t VertexLayout::GetElementIndexInBuffer(int Loc,int Eleindex)
	{
		int index = 0;

		for (int i = 0; i < Loc; i++)
			index = index + Specs[i].NumberOfComponents;

		index = index + Eleindex; //Well Element Number 

		return index;
	}
	std::size_t VertexLayout::GetVertexLength()
	{
		int Length = 0;
		for (auto i : Specs)
			Length += i.NumberOfComponents;
		return Length;
	}
	std::size_t VertexLayout::GetVertexSize()
	{
		int Size = 0;
		for (auto Spec : Specs)
			Size = Size + (Spec.NumberOfComponents * GetSizeOfTheComponentType(Spec.TypeOfTheComponent));
		
		return Size;
	}


	void VertexLayout::UpdateStride()
	{
		int Tracker = 0;

		for (auto Spec : Specs)
		{
			//Update Stride..
			Spec.Stride = Tracker;
			Spec.StrideUpdated = true;

			//Update The Tracker To Goto The Next Specification
			Tracker = Tracker + (Spec.NumberOfComponents * GetSizeOfTheComponentType(Spec.TypeOfTheComponent));
		}
	}


	void VertexLayout::AddSpecification(int NumberOfComponents, unsigned int TypeOfComponent, bool Normalized)
	{
		Specs.emplace_back(Specification{ (int)NumberOfComponents,(unsigned int)TypeOfComponent,(bool)Normalized }); 
		UpdateStride();
	}
	void VertexLayout::AddSpecification(Specification& Spec)
	{
		Specs.emplace_back(Spec);
		UpdateStride();
	}


	EVertexDataObjectType VertexLayout::GetElementType(int elementNumber)
	{
		for (auto i : Specs)
		{
			elementNumber = elementNumber - i.NumberOfComponents;
			if (elementNumber <= 0)
				return EVertexDataObjectType(i.TypeOfTheComponent);
		}
		STDOUTDefaultLog_Error("Error With elementNumber or The Function VertexLayout::GetElementType(int elementNumber)");
		__debugbreak();
	}

	bool VertexLayout::operator==(const VertexLayout& rhs)
	{
		if(!(Specs.size() == rhs.Specs.size()))
			return false;

		for (int i = 0; i < Specs.size(); i++)
		{
			if (Specs[i].TypeOfTheComponent == rhs.Specs[i].TypeOfTheComponent)
			{
				if (Specs[i].NumberOfComponents == rhs.Specs[i].NumberOfComponents)
				{
					if (Specs[i].Normailized == rhs.Specs[i].Normailized)
						continue;
					else return false;
				}
				else return false;
			}
			else return false;
		}

		return true;
	}
}