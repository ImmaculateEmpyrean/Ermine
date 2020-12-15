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
		//Well If You Really Wanted You Can Have A Null Shape Right.. Just Not A Default Constructed One Obviously..
		ShapeBase();

		//If You Really Wanted To Avoid Giving It A Texture..
		ShapeBase(std::vector<Ermine::VertexTextured> Vertexes);

		//This Is The Basic Constructor.. And Can Initialize The Shape Properly..
		ShapeBase(std::vector<std::shared_ptr<Ermine::Sprite>> SpriteBuffer, std::vector<Ermine::VertexTextured> Vertexes);

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
		virtual std::vector<Ermine::VertexAttribPointerSpecification> GetVertexArraySpecification();
		virtual std::vector<uint32_t> GenerateModelSpaceIndices() override;
		virtual int32_t GetRendererDrawMode() override; 
#pragma endregion
		//This Function Has To Be Overriden In all Children Do Not Forget Otherwise One Child May Be Thought Of As The Other..
		virtual Ermine::ActorFamilyIdentifier GetActorFamilyIdentifier() { return ActorFamilyIdentifier::ShapeBase; }

		std::vector<Ermine::VertexTextured> GetAllVertexes();
		std::vector<Ermine::VertexTextured> GetVertexesInRange(int Start, int End);
		Ermine::VertexTextured GetVertex(int Index);
		int GetNumberOfVerticesStored();

		void EmplaceBackVertex(Ermine::VertexTextured Vertex);
		void EmplaceBackVertexes(std::vector<Ermine::VertexTextured> Vertexes);

		void InsertVertex(int index, Ermine::VertexTextured Vertex);
		void InsertVertex(int StartIndex, std::vector<Ermine::VertexTextured> Vertex);

	public:

	protected:

	protected:

	private:

	private:

		//A Holder For All The Vertexes Of This Shape.. We Will USe These Vertices To Draw Said Shape On Screen
		std::vector<Ermine::VertexTextured> Vertexes;

		//This Is Set When The Index Buffer Is Requested.. It Should Be Set Sometime Right.. We Could Set With Vertex Buffer But That Would Probably Be Even More Confusing To Debug..
		int32_t RendererDrawMode = -9999;
	};
}