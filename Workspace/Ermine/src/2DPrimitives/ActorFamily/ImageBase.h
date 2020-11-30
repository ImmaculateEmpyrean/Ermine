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
		//This Function Has To Be Overriden In all Children Do Not Forget Otherwise One Child May Be Thought Of As The Other..
		virtual Ermine::ActorFamilyIdentifier GetActorFamilyIdentifier() { return ActorFamilyIdentifier::ImageBase; }

		//The ImageBase Provides Some Functions To Interact With The Sprite It Stores..
		std::shared_ptr<Sprite> GetSprite();
		void SetSprite(std::shared_ptr<Sprite> Sprite);

		glm::vec2 GetTopRightUV();
		glm::vec2 GetBottomLeftUV();

	public:

	protected:
		static std::shared_ptr<Ermine::Sprite> GenSprite(std::filesystem::path TexturePath, glm::vec2 BottomLeft, glm::vec2 TopRight);

	protected:
		
	private: 
		

	private:
		//This Must Be Able to Be Set Manually By A Child Class.. edit -It Will Be Done USing Method Calls Not Exactly INteraction With The Object
		std::shared_ptr<Sprite> Actorsprite = nullptr;
		

		friend class Ermine::Renderable2D;
		friend class Ermine::RenderableTextureModule;
	};
}