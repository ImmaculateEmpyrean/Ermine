#pragma once
#include<iostream>
#include<vector>
#include<string>

#include "ImageBase.h"
#include "2DPrimitives/Constructs/MovableObject.h"

#include "tmxparser.h"

namespace Ermine
{
	class TiledActor2D : public ImageBase ,public MovableObject
	{
	public:
		//No Point In An Empty Actor Right..
		TiledActor2D() = delete;

		//This Is The Default FilePath..
		TiledActor2D(std::filesystem::path TmxFilePath);

	public:

#pragma region VirtualInterfaceFunctions
		//This Function Is Used To Get The Centre Of Any Actor On Screen.. 
		virtual glm::vec2 GetScreenLocation() override;
#pragma endregion

#pragma region RenderableGenerationImperatives
		//This Function Is Essential For Interaction With Renderable And Its Implemntation Is Necessary For The Concretization Of Any Class..
		virtual glm::mat4 GetModelMatrix() override;
		virtual std::vector<float> GenerateModelSpaceVertexBuffer() override;
		virtual std::vector<Ermine::VertexAttribPointerSpecification> GetVertexArraySpecification() override;
		virtual std::vector<uint32_t> GenerateModelSpaceIndices() override;

#pragma endregion
	public:

	protected:

	protected:

	private:

	private:


	};
}