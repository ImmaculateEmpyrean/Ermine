#pragma once
#include<iostream>
#include<string>
#include<vector>

#include "ImageBase.h"
#include "2DPrimitives/Constructs/MovableObject.h"
#include "glm.hpp"

namespace Ermine {

	//An Actor Is Something That Is Displayed And Movable Nothing Else.. Do Not Asume It Contains Only one Sprite Or Any Such Nonesense..
	//This Is The Non Physics Variant Of tHe Actor.. It Is Moved On The screen Without The Help Of Box2D.. For The Aid Of Physics In Moving Something On The Screen Goto PhysicsActor2D..

	//The Actor Class Is Always Described With Respect To Center Of The Quad..
	class Actor2D : public ImageBase ,public MovableObject
	{
	public:
		//Having An Actor Without a Sprite At This Point Is Quiet Dangerous..
		Actor2D() = delete;

		//Atleast a sprite is required to construct an actor 
		Actor2D(std::shared_ptr<Sprite> Spr);

		//More Sprites The Better When Constructing The Actor..
		Actor2D(std::vector<std::shared_ptr<Sprite>> SpriteBuffer);

		//This is The Default And Best Constructor
		Actor2D(std::shared_ptr<Sprite> Spr, glm::mat4 ModelMatrix);

		//This is The Default and Best Constructor For SpriteBook
		Actor2D(std::vector<std::shared_ptr<Sprite>> SpriteBuffer, glm::mat4 ModelMatrix);

		//This Is Done For Future Proofing Purpose.. Its Children May One Day Choose To Manage Memory On Their Own..
		virtual ~Actor2D();

	public:
		//This Function Comes From Inside The Movement Module Of The Actor.. It is Overriden As We Want To To Turn Off Rotation And Scaling Functionality..
		//The Rotation And scale Functionality Is Essentially Turned Off Because The Default Rotation And scaling Function Of The Actor Is With respect To the Centre Of The Actor And Not The World Centre..
		//The MovementComponent Defaults To Rotating, Scaling And Translating On The Origin of The World We Wish To Avoid That.. i.e it is overriden..
		virtual glm::mat4 GetModelMatrix() override;

		//This Function Has To Be Overriden In all Children Do Not Forget Otherwise One Child May Be Thought Of As The Other..
		virtual Ermine::ActorFamilyIdentifier GetActorFamilyIdentifier() override { return ActorFamilyIdentifier::Actor2D; }

		virtual std::vector<float> CalculateModelSpaceVertexes() override;
		
		virtual glm::vec2 GetScreenLocation() override;

	public:

	protected:

	protected:

	private:
		//This Function Initializes The Renderable2D Part Of The Object When Called.. it is Generally Called By The Renderer.. So That The Vertex Array Is Reset To Reflect Changes In The Model Spaces..
		//virtual void RefreshRenderable2D() override;

		//This Function Is Privated As There Is No Reason Anyone Must Know Or Call This Explicitly..
		//This Function Is Contained Inside The Renderable TextureModule Actually And Is A Public Function inside That Class.. So Eventhough Its a Private Function Here.. It Can Be Called From There With Minimal Effort
		//As Of Now The this Is The Function The Renderer Calls to Bind The Textures If A TextureModule Is Found..vThink Of It As A Callback.. 
		//virtual std::vector<int> BindTexturesContained() override;

	private:


	};

}