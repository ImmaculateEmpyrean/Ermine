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

#include "2DPrimitives/ActorFamily/Actor2DBase.h"
#include "2DPrimitives/ActorFamily/Actor2D.h"

//Maybe We Need a Material System Instead on a standrad shader system   //#include "Graphics/Renderer/MaterialSystem/Shader.h"

namespace Ermine
{
	class Renderable2D : public Ermine::Object
	{
		//Made This Protected As We Expect Child Generate Calls To Call It..
	protected:
		Renderable2D(std::shared_ptr<Ermine::Actor2DBase> Ptr);
		
	public:
		//A Virtual Destructor Ensures Smooth Function Calling From The Com Pointer If Ata All Necessary..
		virtual ~Renderable2D();

#pragma region CopyAndMove
		Renderable2D(Renderable2D& rhs);
		Renderable2D& operator=(Renderable2D& rhs);

		Renderable2D(Renderable2D&& rhs);
		Renderable2D& operator=(Renderable2D&& rhs);
#pragma endregion

	public:
		static std::unique_ptr<Renderable2D> Generate(std::shared_ptr<Actor2DBase> Act);

		std::shared_ptr<VertexArray>  GetVertexArray();
		void SetVertexArray(VertexArray& Vao);

		std::shared_ptr<Material> GetMaterial();
		void SetMaterial(Material& Shd);
		
		//Clear The Vertex Array And The Material As Of This Class.. In Children This Must Behave Differently..
		virtual void Clear();

		//This Function Will Be Called At The First Time A Tick Is Called On It.. And Thats It.. It Is Never Called Again.
		virtual void Initialize(); //Donot Forget To Call This When Overriden In The Future

		//This Function Will Be Called By The Renderer Every Frame.. Override This If Some Function Is To Be Performed EveryFrame..
		virtual void Refresh(float DeltaTime); //Donot Forget To Call This When Overriden In The Future

		std::shared_ptr<Ermine::Actor2DBase> GetBoundActor();

		//Get Or Set The Object Is Initialized State
		void SetObjectInitialized() { ObjectInitialized = true; }
		bool GetObjectInitialized() { return ObjectInitialized; }

		//Get The Model Matrix To be Uploaded To The Gpu..
		glm::mat4 GetModelMatrix();
		//The Binding Maybe Done Differently Depending On The Renderable.. For Example TextureModule May Want To Bind Texture Too.
		virtual void BindRenderable();
		
	public:

	protected:

	protected:

	private:
		//These Functions Try To Recieves The Tick Event And OnBeginEvent From The Object..
		virtual void OnRenderTickEventRecieved(float DeltaTime) override;
		virtual void OnBeginEvent(std::shared_ptr<void> Packet)override;

#pragma region SubGenerateFunctions
		//Let Us Generate A Renderable For The Bound Actor2D Type Actor..
		//void GenerateActor2DRenderable(std::shared_ptr<Ermine::Actor2D> Ptr);

#pragma endregion

#pragma region Helpers
		//Useful For Refreshing Actor2D Type Bounded Actors
		void CopyHelper(Ermine::Renderable2D& rhs);
		void MoveHelper(Ermine::Renderable2D&& rhs);

#pragma endregion
	private:
		//The Vertex Buffer And The Index Buffer Are Initialized Possibly In The Initialize Function..
		std::vector<float> VertexBufferBuffer;
		std::vector<uint32_t> IndexBufferBuffer;

		//This Is The Most Important Part Of The Renderable As Of Now..
		glm::mat4 ModelMatrix = glm::mat4(1.0f);

		//The Vao And Shader Bound While Drawing..
		std::shared_ptr<VertexArray> Vao;
		std::shared_ptr<Material> Mat; 
		//The Vertex Specification Is Given In This VertexAttribPointer Specification..
		std::vector<Ermine::VertexAttribPointerSpecification> Specification;

		//This Is The Actor Which Is To Be Used To Generate Said Renderable..
		std::shared_ptr<Ermine::Actor2DBase> Actor_Bound;

		//This Is A Custom Initializer Function.. Give it A Function If You Wanna Call It Instead Of Standrad Initialize..
		std::function<void()> InitializeFunctionPtr = nullptr;
		bool CallDefaultInitializer = true;

		//This Is A Custom Refresh Function..
		std::function<void()> RefreshFunction = nullptr; //currently not in use.. dummy
		bool CallDefaultRefresh = true; //currently not in use.. dummy

		//This Flag Will Be Used To Call The Initilize Function Once The Object Is Constructed Completely..
		std::once_flag CallInitializeFlag;
		//This Flag Will Be Set To True Once The Entire Object Initialization Is Complete..
		bool ObjectInitialized = false;
	};

}