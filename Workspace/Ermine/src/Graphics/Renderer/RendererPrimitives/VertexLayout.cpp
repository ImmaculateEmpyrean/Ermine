#include "stdafx.h"
#include "VertexLayout.h"

#include<unordered_map>

//TODO: Get Rid Of These Two Headers.. They Have Absolutely No Business Here..
#include<glad/glad.h>
#include<GLFW/glfw3.h>

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
	
	int VertexLayout::GetVertexSize()
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
		Specs.emplace_back(Specification{ NumberOfComponents,TypeOfComponent,Normalized }); 
		UpdateStride();
	}
	void VertexLayout::AddSpecification(Specification& Spec)
	{
		Specs.emplace_back(Spec);
		UpdateStride();
	}

}

