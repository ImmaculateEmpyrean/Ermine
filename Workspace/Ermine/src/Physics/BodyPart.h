#pragma once
#include<iostream>
#include<string>
#include<vector>

#include<box2d/box2d.h>
#include "Physics.h"

namespace Ermine
{
	//This Struct Is Passed Into Ermine To Construct A Part Of The Body in Box2D..
	struct BodyPart
	{
	public:
		//There Is Absolutely No Need Of An Empty Box2DBodyPart
		BodyPart() = delete;

		//This Is The Only Constructor Please Use It To Instantiate The Object..
		BodyPart(b2FixtureDef FixtureDefinition, b2Shape* Shape, glm::vec2 PartSize, 
				 glm::vec2 OffsetFromTheCentre = glm::vec2(0.0f, 0.0f), bool InBox2DSpace = false);

		b2FixtureDef FixtureDefinition;
		b2Shape* Shape;

		glm::vec2 OffsetFromTheCentre;
		glm::vec2 PartSize;

		bool InBox2DSpace = false;

	public:
		void ConvertOffsetFromCentreAndPartSizeToBox2DSpace();

	};
}