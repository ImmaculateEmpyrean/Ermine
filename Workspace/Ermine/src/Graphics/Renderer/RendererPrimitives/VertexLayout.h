#pragma once
#include<iostream>
#include<vector>
#include<string>

#include "EVertexDataObjectType.h"

namespace Ermine
{
	class VertexArray;

	class VertexLayout
	{
	public:
		VertexLayout() = default;
		
	public:
		struct Specification {
			//The Stride Update Is Updated By An Outside Function Hence The Bool Is Used To Keep Track Of If The Stride Is Updated..
			unsigned int Stride = 0;
			bool StrideUpdated = false;

			int NumberOfComponents;
			unsigned int TypeOfTheComponent;
			bool Normailized = false;
		};

		template <typename T, typename... Types>
		VertexLayout(T var1, Types... var2)
		{
			Specs.emplace_back(var1);
			AddSpecifications(var2...);
		}

	public:
		template <typename T, typename... Types>
		void AddSpecifications(T var1, Types... var2)
		{
			Specs.emplace_back(var1);
			AddSpecifications(var2...);
		}

		void AddSpecification(int NumberOfComponents, unsigned int TypeOfComponent, bool Normalized = false); 
		void AddSpecification(Specification& Spec); 

		Specification& GetSpecification(int index) { return Specs[index]; };
		std::vector<Specification>& GetSpecifications() { return Specs; };

		void UpdateSpecification(); //This Call Binds The Specification To The Current Vertex Array IN openGl..

		std::size_t GetElementIndexInBuffer(int Location, int ElementIndex = 0);
		std::size_t GetSpecificationLength(int LocationNumber) { return Specs[LocationNumber].NumberOfComponents; };
		std::size_t GetVertexLength();
		std::size_t GetVertexSize();

		//This function can be used to determine what type a certain element is expected to be to conform to the layout..
		EVertexDataObjectType GetElementType(int elementNumberInBuffer);

		bool operator==(const VertexLayout& rhs);
	private:
		//This Function Initializes The VertexSpecifics By Setting Up The Specifications Start Location..
		void UpdateStride();

	private:
		std::vector<Specification> Specs;

		friend class Ermine::VertexArray;
	};
}