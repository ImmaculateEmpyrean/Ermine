#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<memory>

#include "Graphics/Renderer/RendererPrimitives/VertexArray.h"
#include "Graphics/Renderer/RendererPrimitives/VertexBuffer.h"
#include "Graphics/Renderer/RendererPrimitives/IndexBuffer.h"

#include "Graphics/Renderer/MaterialSystem/Material.h"

#include "Object.h"

//Maybe We Need a Material System Instead on a standrad shader system   //#include "Graphics/Renderer/MaterialSystem/Shader.h"

namespace Ermine
{
	class Renderable2D : public Ermine::Object
	{
	public:
		Renderable2D(); //An Empty Renderable Can Exist Please See That Something Default Exists For Debugging Purposes..
		Renderable2D(VertexArray Vao);
		Renderable2D(Material Mat);
		Renderable2D(VertexArray Vao, Material Mat);

		virtual ~Renderable2D();

	public:
		std::shared_ptr<VertexArray>  GetVertexArray();
		void SetVertexArray(VertexArray& Vao);

		std::shared_ptr<Material> GetMaterial();
		void SetMaterial(Material& Shd);
		std::shared_ptr<Material> GetMaterialBeingUsed();
		
		void Bind(); //Check If bind HAs To Be Virtual..

		//Clear The Vertex Array And The Material As Of This Class.. In Children This Must Behave Differently..
		virtual void Clear();

		//This Function Will Be Called At The First Time A Tick Is Called On It.. And Thats It.. It Is Never Called Again.
		virtual void Initialize() {};
		//This Function Will Be Called By The Renderer Every Frame.. Override This If Some Function Is To Be Performed EveryFrame..
		virtual void Refresh() {};

	public:

	protected:

	protected:

	private:
		void RecieveEvents(Ermine::Event* Eve);

	private:
		//The Vao Which Must Be Bound While Drawing...
		std::shared_ptr<VertexArray> Vao;

		//The Shader Which Is Bound While Drawing...
		std::shared_ptr<Material> Mat; 

		//This Flag Will BE Used To Call The Initilize Function Once The Object Is Constructed Completely..
		std::once_flag CallInitializeFlag;
		//This Flag Will BE Set To True Once The Entire Object Initialization Is Complete..
		bool ObjectInitialized = false;
	};

}