#pragma once
#include<iostream>
#include<vector>
#include<string>

#include <glm.hpp>

/*
	This Struct is To Be Passed To The App Compulsorily For The Initialization And Subseqyent Setting Of The Box2D World..
*/

namespace Ermine
{
	struct PhysicsWorldInitializationStruct
	{
		//The Default Is Set To Mirror Earth Just Rememeber This a New Universe.. 
		glm::vec2 Gravity = glm::vec2(0.0f, -10.0f);
	};
}