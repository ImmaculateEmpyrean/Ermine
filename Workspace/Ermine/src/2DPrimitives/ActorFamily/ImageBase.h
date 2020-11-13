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

		//We Now Know That This Is A Quad.. Hence Return The Indices which Make The Quad Possible..
		virtual std::vector<uint32_t> GetIndices() override;

		//We Now Know That This Is A Quad.. Hence Return The Vertexes That Make Drawing This Actor Possible..
		virtual std::vector<float> CalculateModelSpaceVertexes() override;

#pragma region RenderableTextureModuleExposition
		//The Texture Path Is Submitted To The GlobalTextureCache From which a Texture Is Recieved For The Buffer
		virtual void SubmitTexture(std::filesystem::path TexturePath) override;

		//Submit A Texture To Be Held By The Renderable Texture Module..
		virtual void SubmitTexture(std::shared_ptr<Texture> Texture) override;
		
		//Unable To Implemnt This In Current Archietecture
		//A Handle To the Textures Buffer To Modify It In Some Sense.. Note Renderable Texture Module Does Not Have A Delete Function..
		//virtual std::vector<std::shared_ptr<Texture>>& GetBuffer() override;

		//Binds Textures Contained Inside The Renderable Texture Module And Returns An Array Containing In which Slot Each Texture Is Bound..
		//virtual std::vector<int> BindTexturesContained() override; This Is Already Overriden In ImageBase.h

		//Clears Out The Contents Of The RenderableTextureModuleBuffer And Its PARENTS TOO...
		virtual void Clear() override;

		//Clear Out Only The Texture Buffer Of The RenderableTextureModule..
		virtual void ClearTextureBuffer() override;
#pragma endregion

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
	};
}