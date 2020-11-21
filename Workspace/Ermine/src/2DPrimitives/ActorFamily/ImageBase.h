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

		//We Now Know That This Is A Quad.. Hence Return The Indices which Make The Quad Possible..
		//virtual std::vector<uint32_t> GetIndices() override;

		//We Now Know That This Is A Quad.. Hence Return The Vertexes That Make Drawing This Actor Possible..
		//virtual std::vector<float> CalculateModelSpaceVertexes() override;

#pragma region RenderableTextureModuleExposition
		/*//The Texture Path Is Submitted To The GlobalTextureCache From which a Texture Is Recieved For The Buffer
		virtual void SubmitTexture(std::filesystem::path TexturePath) override;

		//Submit A Texture To Be Held By The Renderable Texture Module..
		virtual void SubmitTexture(std::shared_ptr<Texture> Texture) override;
		
		//Clears Out The Contents Of The RenderableTextureModuleBuffer And Its PARENTS TOO...
		virtual void Clear() override;

		//Clear Out Only The Texture Buffer Of The RenderableTextureModule..
		virtual void ClearTextureBuffer() override;*/
#pragma endregion

	public:

	protected:

	protected:

	protected:
		//This Function Initializes The Renderable2D Part Of The Object When Called.. it is Generally Called By The Renderer.. So That The Vertex Array Is Reset To Reflect Changes In The Model Spaces..
		//virtual void RefreshRenderable2D();

		//This Function Gives Us Information Of The Vertex Array attributes associated With The VertexArray In Question.. 
		virtual std::vector<VertexAttribPointerSpecification> GetVertexAttribSpecificationForTheActor() override;

		//This Function Is Privated As There Is No Reason Anyone Must Know Or Call This Explicitly..
		//This Function Is Contained Inside The Renderable TextureModule Actually And Is A Public Function inside That Class.. So Eventhough Its a Private Function Here.. It Can Be Called From There With Minimal Effort
		//As Of Now The this Is The Function The Renderer Calls to Bind The Textures If A TextureModule Is Found..vThink Of It As A Callback.. 
		//virtual std::vector<int> BindTexturesContained() override;

		//The Refresh Is Important As It Is Required To Generate The Renderable To Be Drawn On The Screen..
		//virtual void Refresh() override;
	private: 
		

	private:
		//This Must Be Able to Be Set Manually By A Child Class.. edit -It Will Be Done USing Method Calls Not Exactly INteraction With The Object
		std::shared_ptr<Sprite> Actorsprite;

		friend class Ermine::Renderable2D;
		friend class Ermine::RenderableTextureModule;
	};
}