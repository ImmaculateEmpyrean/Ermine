#pragma once
#include<iostream>
#include<string>
#include<vector>

#include "Constructs/Quad.h"
#include "glm.hpp"
#include "Sprite.h"


namespace Ermine {

	//The Actor Class Is Always Described With Respect To Center Of The Quad..
	class Actor2D
	{
	public:
		//Having An Actor Without a Sprite At This Point Is Quiet Dangerous..
		Actor2D() = delete;

		//Atleast a sprite is required to construct an actor 
		Actor2D(std::shared_ptr<Sprite> Spr);

		//This is The Default And BEst Constructor
		Actor2D(std::shared_ptr<Sprite> Spr, glm::mat4 ModelMatrix);

		//This Class Owns And Manages Pointers
		~Actor2D();

	public:
		std::shared_ptr<Sprite> GetSprite();

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

	public:

	protected:

	protected:

	private:

	private:
		glm::mat4 RecievedModelMatrix; //This Is Related To The World
		std::shared_ptr<Sprite> Actorsprite; //This Sprite Is Owned By The Actor And Is Freed When The Actor2D Is Destroyed Probably..

		glm::mat4 TranslationMatrix;
		glm::mat4 RotationMatrix;
		glm::mat4 ScaleMatrix;

	};

}