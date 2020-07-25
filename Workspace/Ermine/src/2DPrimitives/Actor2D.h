#pragma once
#include<iostream>
#include<string>
#include<vector>

#include "Graphics/Renderer/RendererPrimitives/VertexArray.h"

#include "Constructs/Quad.h"
#include "glm.hpp"

#include "2DPrimitives/Sprite.h"
#include "Renderable2D.h"


namespace Ermine {

	//An Actor Is Something That Is Displayed And Movable Nothing Else.. Do Not Asume It Contains Only one Sprite Or Any Such Nonesense..

	//The Actor Class Is Always Described With Respect To Center Of The Quad..
	class Actor2D : public Renderable2D
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
		void Translate(float x, float y);
		void Translate(glm::vec2 TranslateByHowMuch);
		void ClearTranslations();

		//Just Pass In True If The Angle Is Actually Pi Most Probably It Is In Degrees...(if false is passed then i assume DEGREES).
		void Rotate(float Angle,bool IsInPI=false);
		void ClearRotations();

		void Scale(float x, float y);
		void Scale(glm::vec2 ScaleByHowMuch);
		void ClearScale();

		std::vector<float> GetModelSpaceCoordinates();
		static std::vector<uint32_t> GetModelSpaceIndices();

		glm::mat4 GetModelMatrix();

		std::shared_ptr<Sprite> GetSprite();
		void SetSprite(std::shared_ptr<Sprite> Sprite);

		virtual Renderable2DType GetType() { return Renderable2DType::ACTOR2D; }

	public:

	protected:

	protected:

	private:
		void HelperInitializeRenderable2D();

	private:
		glm::mat4 RecievedModelMatrix; //This Is Related To The World
		std::shared_ptr<Sprite> Actorsprite; //This Sprite Is Owned By The Actor And Is Freed When The Actor2D Is Destroyed Probably..

		glm::mat4 TranslationMatrix;
		glm::mat4 RotationMatrix;
		glm::mat4 ScaleMatrix;

	};

}