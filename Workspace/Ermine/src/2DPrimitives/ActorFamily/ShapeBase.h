#pragma once
#include<iostream>
#include<vector>
#include<string>

#include<memory>

#include "Actor2DBase.h"
#include "ImageBase.h"

#include "2DPrimitives/Constructs/VertexTextured.h"

namespace Ermine
{
	class ShapeBase: public Ermine::ImageBase
	{
	protected:
		//Well Image Base Cannot Be Default Initialized.. So We Have No Choice
		ShapeBase() = delete;

		//This Is The Basic Constructor.. And Can Initialize The Shape Properly..
		ShapeBase(std::shared_ptr<Ermine::Sprite> Sprite, std::vector<Ermine::VertexTextured> Vertexes);

	public:
		//The Destructor Is Used TO Dispatch Destruction Calls Appropriately..
		virtual ~ShapeBase();

		ShapeBase(ShapeBase& rhs);
		ShapeBase& operator=(ShapeBase& rhs);

		ShapeBase(ShapeBase&& rhs);
		ShapeBase& operator=(ShapeBase&& rhs);

	public:
#pragma region VirtualInterfaceFunctions
		//This Function Is Used To Get The Centre Of Any Actor On Screen.. 
		virtual glm::vec2 GetScreenLocation() = 0;
#pragma endregion

#pragma region RenderableGenerationImperatives
		//This Function Is Essential For Interaction With Renderable And Its Implemntation Is Necessary For The Concretization Of Any Class..
		virtual glm::mat4 GetModelMatrix() = 0; //This Cannot Be Implemnted Here As Shape Base Has No Movemnt Component
		virtual std::vector<float> GenerateModelSpaceVertexBuffer() override;
		virtual std::vector<Ermine::VertexAttribPointerSpecification> GetVertexArraySpecification() override;
		virtual std::vector<uint32_t> GenerateModelSpaceIndices() override;
#pragma endregion

		std::vector<Ermine::VertexTextured> GetAllVertexes();
		std::vector<Ermine::VertexTextured> GetVertexesInRange(int Start, int End);
		Ermine::VertexTextured GetVertex(int Index);

		void EmplaceBackVertex(Ermine::VertexTextured Vertex);
		void EmplaceBackVertexes(std::vector<Ermine::VertexTextured> Vertexes);

		void InsertVertex(int index, Ermine::VertexTextured Vertex);
		void InsertVertex(int StartIndex, std::vector<Ermine::VertexTextured> Vertex);

	public:

	protected:

	protected:

	private:

	private:
		std::vector<Ermine::VertexTextured> Vertexes;

	};
}