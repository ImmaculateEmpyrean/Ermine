#pragma once
#include<iostream>
#include<string>
#include<vector>

#include "Vertex.h"
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
		Actor2D(Sprite* Spr);

		//This is The Default And BEst Constructor
		Actor2D(Sprite* Spr, glm::mat3 ModelMatrix);

		//This Class Owns And Manages Pointers
		~Actor2D();

	public:
		void Translate(int x, int y);
		void Translate(glm::vec2 TranslateByHowMuch);

		void Rotate(int Pi);
		void Rotate(float Degrees);

		void Scale(int x, int y);
		void Scale(glm::vec2 ScaleByHowMuch);



		std::vector<Vertex> GetVertices();
		glm::mat3 GetModelMatrix();

	public:

	protected:

	protected:

	private:

	private:
		glm::mat3 ModelMatrix; //This Is Related To The World
		Sprite* Actorsprite; //This Sprite Is Owned By The Actor And Is Freed When The Actor2D Is Destroyed..

		glm::mat3 TranslationMatrix;
		glm::mat3 RotationMatrix;
		glm::mat3 ScaleMatrix;

	};

}