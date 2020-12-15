#pragma once
#include<iostream>
#include<vector>
#include<string>

#include<memory>
#include<functional>

#include "Actor2DBase.h"

#include "2DPrimitives/Sprite.h"
#include "2DPrimitives/SpriteBook.h"

namespace Ermine
{
	//Forward Declared So That We Can Fix It As Friend.. Not Because Actor Needs To Know About Them Or Anything Like That..
	class Renderable2D;
	class RenderableTextureModule;

	class ImageBase :public Actor2DBase
	{
	public:

#pragma region Constructors
		//No Point In Having An Empty ImageBase But You Could Still Choose To Have One..
		ImageBase();

		//Atleast a sprite is required to construct an actor 
		ImageBase(std::shared_ptr<Sprite> Spr);

		//More Sprites The Better When Constructing The Actor..
		ImageBase(std::vector<std::shared_ptr<Sprite>> SpriteBuffer);

		//A Virtual Destructor For Control To Be Sent To The Children If And when Needed...
		virtual ~ImageBase();

		//Image Base Now Incorporates A Mutex And Hence It Cant Be Copied Or Moved Trivially..//
		ImageBase(ImageBase& rhs);
		ImageBase& operator=(ImageBase& rhs);

		ImageBase(ImageBase&& rhs);
		ImageBase& operator=(ImageBase&& rhs);
		//Image Base Now Incorporates A Mutex And Hence It Cant Be Copied Or Moved Trivially..//
#pragma endregion

	public:
#pragma region RenderableGenerationImperatives
		//This Function Is Essential For Interaction With Renderable And Its Implemntation Is Necessary For The Concretization Of Any Class..
		virtual glm::mat4 GetModelMatrix() = 0;
		virtual std::vector<float> GenerateModelSpaceVertexBuffer() = 0;
		virtual std::vector<Ermine::VertexAttribPointerSpecification> GetVertexArraySpecification() override;
		virtual std::vector<uint32_t> GenerateModelSpaceIndices() = 0;
		virtual int32_t GetRendererDrawMode() override;
#pragma endregion

		//This Function Has To Be Overriden In all Children Do Not Forget Otherwise One Child May Be Thought Of As The Other..
		virtual Ermine::ActorFamilyIdentifier GetActorFamilyIdentifier() { return ActorFamilyIdentifier::ImageBase; }

		//The ImageBase Provides Some Functions To Interact With The Sprite It Stores..
		std::shared_ptr<Sprite> GetSprite(int index =0);
		std::vector<std::shared_ptr<Ermine::Sprite>> GetSpriteBuffer(); //I Cannot Give A Reference.. Refer And Delete Manually..

		void SetSprite(std::shared_ptr<Sprite> Sprite);
		void RemoveSprite(int index);

		glm::vec2 GetTopRightUV(int index = 0);
		glm::vec2 GetBottomLeftUV(int index = 0);

	public:

	protected:
		static std::shared_ptr<Ermine::Sprite> GenSprite(std::filesystem::path TexturePath, glm::vec2 BottomLeft, glm::vec2 TopRight);

	protected:
		
	private: 
		

	private:
		//This Must Be Able to Be Set Manually By A Child Class.. edit -It Will Be Done Using Method Calls Not Exactly Interaction With The Object
		std::vector<std::shared_ptr<Ermine::Sprite>> ActorSprites;

		friend class Ermine::Renderable2D;
		friend class Ermine::RenderableTextureModule;
	};
}