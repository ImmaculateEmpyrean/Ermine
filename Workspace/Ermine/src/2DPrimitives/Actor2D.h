#pragma once
#include<iostream>
#include<string>
#include<vector>

#include "Graphics/Renderer/RendererPrimitives/VertexArray.h"

#include "Constructs/Quad.h"
#include "Constructs/MovableObject.h"
#include "glm.hpp"

#include "2DPrimitives/Sprite.h"
#include "Renderable2D.h"


namespace Ermine {

	//An Actor Is Something That Is Displayed And Movable Nothing Else.. Do Not Asume It Contains Only one Sprite Or Any Such Nonesense..

	//The Actor Class Is Always Described With Respect To Center Of The Quad..
	class Actor2D : public Renderable2D,public MovableObject
	{
	public:
		//Having An Actor Without a Sprite At This Point Is Quiet Dangerous..
		Actor2D() = delete;

		//Atleast a sprite is required to construct an actor 
		Actor2D(std::shared_ptr<Sprite> Spr);

		//This is The Default And BEst Constructor
		Actor2D(std::shared_ptr<Sprite> Spr, glm::mat4 ModelMatrix);

		//This Class Owns And Manages Pointers
		virtual ~Actor2D();

	public:
		

		std::vector<float> GetModelSpaceCoordinates();
		static std::vector<uint32_t> GetModelSpaceIndices();

		std::shared_ptr<Sprite> GetSprite();
		void SetSprite(std::shared_ptr<Sprite> Sprite);

		virtual Renderable2DType GetType() { return Renderable2DType::ACTOR2D; }

	public:

	protected:

	protected:

	private:
		void HelperInitializeRenderable2D();

	private:
		 //This Is Related To The World
		std::shared_ptr<Sprite> Actorsprite; //This Sprite Is Owned By The Actor And Is Freed When The Actor2D Is Destroyed Probably..

		

	};

}