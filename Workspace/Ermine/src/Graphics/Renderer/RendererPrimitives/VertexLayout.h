#pragma once
#include<iostream>
#include<vector>
#include<string>

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

		int GetVertexSize();
	public:
		//This Is Where The Attributes Are Stored..
		std::vector<Specification> Specs;
		std::vector<unsigned int> SpecificationStartLocations;

	private:
		//This Function Initializes The VertexSpecifics By Setting Up The Specifications Start Location..
		void UpdateStride();

		friend class Ermine::VertexArray;
	};
}