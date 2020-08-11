#pragma once
#include<iostream>
#include<vector>
#include<string>

#include "glm.hpp"
#include <box2d/box2d.h>

namespace Ermine
{
	//This Is The MAin World Object To Be Used By Ermine..
	extern b2World* Universum;

	//This Scale Factor Is Responsible 
	extern float ScaleFactor;

	//Start Convert Box2D World Coordinates to Pixel Space Coordinates//
	glm::vec2 coordWorldToPixels(glm::vec2 world);
	glm::vec2 coordWorldToPixels(float worldX, float worldY);
	//Ended Convert Box2D World Coordinates to Pixel Space Coordinates//

	//Start Convert Pixel Space Coordinates to Box2D World Coordinates//
	glm::vec2 coordPixelsToWorld(glm::vec2 screen);
	glm::vec2 coordPixelsToWorld(float pixelX, float pixelY);
	//Ended Convert Pixel Space Coordinates to Box2D World Coordinates//

	//Start Scale scalar quantity between worlds//
	float scalarPixelsToWorld(float val);
	float scalarWorldToPixels(float val);
	//Ended Scale scalar quantity between worlds//

	//Start Scale vector between worlds//
	glm::vec2 vectorPixelsToWorld(glm::vec2 v);
	glm::vec2 vectorPixelsToWorld(glm::vec2 v);
	glm::vec2 vectorPixelsToWorld(float x, float y);
	glm::vec2 vectorWorldToPixels(glm::vec2 v);
	glm::vec2 vectorWorldToPixelsPVector(glm::vec2 v);
	//Ended Scale Vector Between Worlds
}