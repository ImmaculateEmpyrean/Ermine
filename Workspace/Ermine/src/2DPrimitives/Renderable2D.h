#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<memory>

#include "Graphics/Renderer/RendererPrimitives/VertexArray.h"
#include "Graphics/Renderer/RendererPrimitives/VertexBuffer.h"
#include "Graphics/Renderer/RendererPrimitives/IndexBuffer.h"

#include "Graphics/Renderer/MaterialSystem/Material.h"

#include "Renderable2DEnums.h"

//Maybe We Need a Material System Instead on a standrad shader system   //#include "Graphics/Renderer/MaterialSystem/Shader.h"

namespace Ermine
{
	class Renderer2D; //Forward Declaration So That This Can Be Made a Friend Class..

	class Renderable2D
	{
	public:
		Renderable2D(); //An Empty Renderable Can Exist Please See That Something Default Exists For Debugging Purposes..
		Renderable2D(VertexArray Vao);
		Renderable2D(VertexArray Vao, Material Mat);

		virtual ~Renderable2D();

	public:
		void SetVertexArray(VertexArray Vao);
		std::shared_ptr<VertexArray>  GetVertexArray();

		void SetMaterial(Material Shd);
		std::shared_ptr<Material> GetMaterialBeingUsed();
		
		void Bind(); //Check If bind HAs To Be Virtual..

		virtual Renderable2DType GetType() { return Renderable2DType::Renderable2D; }

	public:

	protected:

	protected:

	private:

	private:
		//The Vao Which Must Be Bound While Drawing...
		std::shared_ptr<VertexArray> Vao;

		//The Shader Which Is Bound While Drawing...
		std::shared_ptr<Material> Mat; 

		friend class Ermine::Renderer2D;
	};

}