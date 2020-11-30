#pragma once
#include<iostream>
#include<vector>
#include<string>

#include<glm.hpp>

namespace Ermine
{
	//This Struct Is Used To Store The Debug Trace Color Of A Single Fixture..
	struct FixtureColorStruct
	{
	public:
		void Randomize();

	public:
		//The Numbers Are Stored In 0 - 255 Range.. OpenGl Deals with 0-1 range.. beware..
		glm::vec4 Color = glm::vec4(1.0);
	};
}