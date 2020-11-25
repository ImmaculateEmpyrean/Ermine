#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<memory>
#include<filesystem>

#include "glm.hpp"

#include "Object.h"
#include "2DPrimitives/ActorFamily/Actor2DBase.h"

namespace Ermine
{
	//This Callback Can Be Implemnted In The Game If It Wants To Send In Some Custom Constructed Actors..
	std::shared_ptr<Ermine::Actor2DBase> Actor2DConstructionCallback(std::shared_ptr<Ermine::Actor2DBase> Act);

	class Level: public Object
	{
	public:
		//It Is Dangerous To Have A Level With Absolutely No Configuration..
		Level() = delete;

		//Use This Constructor The Most.. It is The Recommended And Only Supported One For Now..
		Level(std::filesystem::path LevelPath);

		//There Is Little Point In Copying A Level.. So We Premptively Disabled Said Copy..
		Level(const Level& rhs) = delete;
		Level& operator=(const Level& rhs) = delete;

		//The Level Can Absolutely Be Moved Around This Is How We Actually Get The Level In The Proper Place..
		Level(Level&& rhs);
		Level& operator=(Level&& rhs);

	public:
		std::vector<std::shared_ptr<Ermine::Actor2DBase>> GetActors();
		glm::vec2 GetGravityVector();

	public:
		//This Is Pretty Expensive See That You Dont Use It Too Often..
		void LoadLevel();

	protected:

	protected:

	private:

	private:
		//This is probably not required.. but lets just keep it around..
		std::filesystem::path FilePath;

		//This Is The World Gravity and Is Set By The Level.. (Case Can Be Made That The Game Mode Is InCharge Lets See....)
		glm::vec2 WorldGravity = glm::vec2(0.0f,-10.0f);

		//These Are The Actors In This Scene.. Which Are To be Initialized And Then Submitted To The Renderer..
		std::vector<std::shared_ptr<Ermine::Actor2DBase>> ActorBuffer;
	};
}