#include "stdafx.h"
#include "Physics.h"

#include<iostream>
#include<vector>
#include<string>

//The Variable Is Defined in this translation unit as it could not be defined in a header file..
b2World* Ermine::Universum = nullptr;

//This Variable Is Used To Convert To And From The Pixel Space..
static float Ermine::ScaleFactor = 0.01f;

//This Is Used To Set The Time Which The Physics World Advances Every Frame..
float Ermine::PhysicsWorldTimestep = 1.0f / 60.0f;

//Not Exactly Sure Looks Like Some Physics Stuff.. Found Out On The Internet That These Are Acceptable Default Values..
int32 Ermine::PhysicsVelocityIterations = 6;
int32 Ermine::PhysicsPositionIterations = 2;

//These Variables Hold Information Abt The extant Of The Physical World..
// Donot Forget To Update Them Whenever The Scale Factor Has Changed Or ScreenWidth Or Height Has Changed
glm::vec2 Ermine::PhysicsWorldTopLeft; 
glm::vec2 Ermine::PhysicsWorldBottomRight;

namespace Ermine
{
	void RecalculatePhysicsWorldBounds()
	{
		//It Does What It Does Note- Physics World Is Not The Same As Pixel World keep That In Mind Before Trying To Change The Function..
		PhysicsWorldTopLeft = glm::vec2((-1.0f * (Ermine::GetScreenWidth() / 2.0f) * ScaleFactor, (Ermine::GetScreenHeight() / 2.0f)) * ScaleFactor);
		PhysicsWorldBottomRight = glm::vec2((Ermine::GetScreenWidth() / 2.0f) * ScaleFactor, (-1.0f * (Ermine::GetScreenHeight() / 2.0f)) * ScaleFactor);
	}


	//Start Vector Conversion Functions From Vec2 To B2Vec2
	glm::vec2 B2Vec2ToGLM(b2Vec2 Vector)
	{
		return glm::vec2(Vector.x, Vector.y);
	}
	b2Vec2 GLMToB2Vec2(glm::vec2 Vector)
	{
		return b2Vec2(Vector.x, Vector.y);
	}
	//Ended Vector Conversion Functions From Vec2 To B2Vec2

	glm::vec2 coordWorldToPixels(glm::vec2 world)
	{
		world.x = world.x / ScaleFactor;
		world.y = world.y / ScaleFactor;

		glm::vec2 PixelSpace;
		PixelSpace.x = world.x + (Ermine::GetScreenWidth() / 2);
		PixelSpace.y = (Ermine::GetScreenHeight() / 2) - world.y;

		return PixelSpace;
	}
	glm::vec2 coordWorldToPixels(float worldX, float worldY)
	{
		//Return the Result of The Previous Function..
		return coordWorldToPixels(glm::vec2(worldX, worldY));	
	}

	glm::vec2 coordPixelsToWorld(glm::vec2 screen)
	{
		glm::vec2 WorldSpace;
		WorldSpace.x = screen.x - (Ermine::GetScreenWidth() / 2);
		WorldSpace.y = (Ermine::GetScreenHeight() / 2) - screen.y;

		WorldSpace.x = WorldSpace.x * ScaleFactor;
		WorldSpace.y = WorldSpace.y * ScaleFactor;

		return WorldSpace;
	}
	glm::vec2 coordPixelsToWorld(float pixelX, float pixelY)
	{
		//Return the Result of The Previous Function..
		return coordPixelsToWorld(glm::vec2(pixelX,pixelY));
	}


	float scalarPixelsToWorld(float val)
	{
		return val * ScaleFactor;
	}
	float scalarWorldToPixels(float val)
	{
		return val / ScaleFactor;
	}


	glm::vec2 vectorPixelsToWorld(glm::vec2 screen)
	{
		glm::vec2 WorldCoordinates;
		WorldCoordinates.x = screen.x * ScaleFactor;
		WorldCoordinates.y = screen.y * ScaleFactor;

		return WorldCoordinates;
	}
	glm::vec2 vectorPixelsToWorld(float pixelX, float pixelY)
	{
		//just use the function on the top of this function
		return vectorPixelsToWorld(glm::vec2(pixelX, pixelY));
	}

	glm::vec2 vectorWorldToPixels(glm::vec2 world)
	{
		glm::vec2 PixelCoordinates;
		PixelCoordinates.x = world.x / ScaleFactor;
		PixelCoordinates.y = world.y / ScaleFactor;

		return PixelCoordinates;
	}
	glm::vec2 vectorWorldToPixels(float worldX,float worldY)
	{
		//Use The result of The Previous function to return to the user.. 
		return vectorWorldToPixels(glm::vec2(worldX, worldY));
	}


	glm::vec2 vertexPixelsToWorld(glm::vec2 screen)
	{
		glm::vec2 WorldCoordinates;
		WorldCoordinates.x = screen.x * ScaleFactor;
		WorldCoordinates.y = screen.y * ScaleFactor;

		WorldCoordinates.y = WorldCoordinates.y * -1.0f;

		return WorldCoordinates;
	}
	glm::vec2 vertexPixelsToWorld(float pixelX, float pixelY)
	{
		return vertexPixelsToWorld(glm::vec2(pixelX, pixelY));
	}
	
	glm::vec2 vertexWorldToPixels(glm::vec2 world)
	{
		glm::vec2 PixelCoordinates;
		PixelCoordinates.x = world.x / ScaleFactor;
		PixelCoordinates.y = world.y / ScaleFactor;

		PixelCoordinates.y = PixelCoordinates.y * -1.0f;

		return PixelCoordinates;
	}
	glm::vec2 vertexWorldToPixels(float worldX, float worldY)
	{
		return vertexWorldToPixels(glm::vec2(worldX,worldY));
	}
}