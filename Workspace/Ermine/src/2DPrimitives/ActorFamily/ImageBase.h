#pragma once
#include<iostream>
#include<vector>
#include<string>

#include<memory>
#include<functional>

#include "Actor2DBase.h"
#include "Graphics/Renderer/RenderableComponents/RenderableTextureModule.h"

#include "2DPrimitives/Sprite.h"
#include "2DPrimitives/SpriteBook.h"

namespace Ermine
{
	class ImageBase :public Actor2DBase, public RenderableTextureModule
	{
	public:

#pragma region Constructors
		//No Point In Having An Empty ImageBase 
		ImageBase() = delete;

		//Atleast a sprite is required to construct an actor 
		ImageBase(std::shared_ptr<Sprite> Spr);

		//More Sprites The Better When Constructing The Actor..
		ImageBase(std::vector<std::shared_ptr<Sprite>> SpriteBuffer);

		//A Virtual Destructor For Control To Be Sent To The Children If And when Needed...
		virtual ~ImageBase();

		//Image Base Now Incorporates A Mutex And Hence It Cant Be Copied Or Moved Trivially..//
		ImageBase(const ImageBase& rhs);
		ImageBase& operator=(const ImageBase& rhs);

		ImageBase(ImageBase&& rhs);
		ImageBase& operator=(ImageBase&& rhs);
		//Image Base Now Incorporates A Mutex And Hence It Cant Be Copied Or Moved Trivially..//
#pragma endregion

	public:

#pragma region IMutexOverrides
		//Start IMutex Overrides//
		virtual std::unique_lock<std::recursive_mutex> GetUniqueLock() override { return std::unique_lock<std::recursive_mutex>(ImageBaseMutex); }
		virtual Ermine::MutexLevel GetMutexLevel() override { return Ermine::MutexLevel::ImageBase; }
		//Ended IMutex Overrides//
#pragma endregion

		//This Function Has To Be Overriden In all Children Do Not Forget Otherwise One Child May Be Thought Of As The Other..
		virtual Ermine::ActorFamilyIdentifier GetActorFamilyIdentifier() { return ActorFamilyIdentifier::ImageBase; }

		//The ImageBase Provides Some Functions To Interact With The Sprite It Stores..
		std::shared_ptr<Sprite> GetSprite() const;
		void SetSprite(std::shared_ptr<Sprite> Sprite);

		//We Now Know That This Is A Quad.. Hence Return The Indices which Make The Quad Possible..
		virtual std::vector<uint32_t> GetIndices() override;

		//We Now Know That This Is A Quad.. Hence Return The Vertexes That Make Drawing This Actor Possible..
		virtual std::vector<float> CalculateModelSpaceVertexes() override;

	public:

	protected:

	protected:

	protected:
		//This Function Initializes The Renderable2D Part Of The Object When Called.. it is Generally Called By The Renderer.. So That The Vertex Array Is Reset To Reflect Changes In The Model Spaces..
		virtual void RefreshRenderable2D();

		//This Function Gives Us Information Of The Vertex Array attributes associated With The VertexArray In Question.. 
		virtual std::vector<VertexAttribPointerSpecification> GetVertexAttribSpecificationForTheActor() override;

		//This Function Is Privated As There Is No Reason Anyone Must Know Or Call This Explicitly..
		//This Function Is Contained Inside The Renderable TextureModule Actually And Is A Public Function inside That Class.. So Eventhough Its a Private Function Here.. It Can Be Called From There With Minimal Effort
		//As Of Now The this Is The Function The Renderer Calls to Bind The Textures If A TextureModule Is Found..vThink Of It As A Callback.. 
		virtual std::vector<int> BindTexturesContained() override;

	private: 
		

	private:
		//This Must Be Able to Be Set Manually By A Child Class.. edit -It Will Be Done USing Method Calls Not Exactly INteraction With The Object
		std::shared_ptr<Sprite> Actorsprite;

		//This Is The Main Mutex Held By The Class In Question.. Must Be Aquired Before Most Methods Can Begin Executing..
		std::recursive_mutex ImageBaseMutex;
	};
}