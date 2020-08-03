#pragma once
#include<iostream>
#include<string>
#include<vector>

#include "Constructs/Quad.h"
#include "Constructs/MovableObject.h"
#include "glm.hpp"

#include "2DPrimitives/PrimitiveType2D.h"
#include "2DPrimitives/Sprite.h"
#include "2DPrimitives/SpriteBook.h"

#include "Graphics/Renderer/RenderableComponents/RenderableTextureModule.h"

namespace Ermine {

	//An Actor Is Something That Is Displayed And Movable Nothing Else.. Do Not Asume It Contains Only one Sprite Or Any Such Nonesense..

	//The Actor Class Is Always Described With Respect To Center Of The Quad..
	class Actor2D : public RenderableTextureModule,public MovableObject
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

		//This Class Owns And Manages Pointers
		virtual ~Actor2D();

	public:
		virtual glm::mat4 GetModelMatrix() override;

		std::vector<float> GetModelSpaceCoordinates();
		static std::vector<uint32_t> GetModelSpaceIndices();

		std::shared_ptr<Sprite> GetSprite();
		void SetSprite(std::shared_ptr<Sprite> Sprite);

		virtual Ermine::PrimitiveType2D GetType() { return PrimitiveType2D::ACTOR2D; }

	public:

	protected:

	protected:

	private:
		void HelperInitializeRenderable2D();

		//This Function Is Privated As There Is No Reason Anyone Must Know Or Call This Explicitly..
		virtual std::vector<int> BindTexturesContained() override;

	private:
		 //This Is Related To The World
		std::shared_ptr<Sprite> Actorsprite; //This Sprite Is Owned By The Actor And Is Freed When The Actor2D Is Destroyed Probably..
	};

}