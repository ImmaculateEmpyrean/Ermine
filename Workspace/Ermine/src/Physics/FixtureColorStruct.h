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
		bool UseColor= false;
		glm::vec4 Color = glm::vec4(1.0);
	};
}