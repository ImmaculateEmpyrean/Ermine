#include "stdafx.h"
#include "Physics.h"

#include<iostream>
#include<vector>
#include<string>

//The Variable Is Defined in this translation unit as it could not be defined in a header file..
b2World* Ermine::Universum = nullptr;

//This Variable Is Used To Convert To And From The Pixel Space..
static float Ermine::ScaleFactor = 100.0f;

//This Is Used To Set The Time Which The Physics World Advances Every Frame..
float Ermine::PhysicsWorldTimestep = 1.0f / 60.0f;

//Not Exactly Sure Looks Like Some Physics Stuff.. Found Out On The Internet That These Are Acceptable Default Values..
int32 Ermine::PhysicsVelocityIterations = 6;
int32 Ermine::PhysicsPositionIterations = 2;

namespace Ermine
{
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

	glm::vec2 coordWorldToErmine(glm::vec2 world)
	{
		world.x = world.x * ScaleFactor;
		world.y = world.y * ScaleFactor;

		glm::vec2 PixelSpace;
		PixelSpace.x = world.x; // +(Ermine::GetScreenWidth() / 2);
		PixelSpace.y = 540.0f - world.y; //Since Ermine Space Goes From 0 to 1080.. And We Have To Flip It Without Getting a Negative Value

		return PixelSpace;
	}
	glm::vec2 coordWorldToErmine(float worldX, float worldY)
	{
		//Return the Result of The Previous Function..
		return coordWorldToErmine(glm::vec2(worldX, worldY));	
	}

	glm::vec2 coordErmineToWorld(glm::vec2 screen)
	{
		glm::vec2 WorldSpace;
		WorldSpace.x = screen.x; // -(Ermine::GetScreenWidth() / 2);
		WorldSpace.y = 540.0f - screen.y; // Since Ermine Space Goes From 0 To 1080 In Y.. And We Have To Flip It Without Getting a Negative Value

		WorldSpace.x = WorldSpace.x / ScaleFactor;
		WorldSpace.y = WorldSpace.y / ScaleFactor;

		return WorldSpace;
	}
	glm::vec2 coordErmineToWorld(float pixelX, float pixelY)
	{
		//Return the Result of The Previous Function..
		return coordErmineToWorld(glm::vec2(pixelX,pixelY));
	}


	float scalarErmineToWorld(float val)
	{
		return val / ScaleFactor;
	}
	float scalarWorldToErmine(float val)
	{
		return val * ScaleFactor;
	}


	glm::vec2 vectorErmineToWorld(glm::vec2 screen)
	{
		glm::vec2 WorldCoordinates;
		WorldCoordinates.x = scalarErmineToWorld(screen.x);
		WorldCoordinates.y = scalarErmineToWorld(screen.y);

		return WorldCoordinates;
	}
	glm::vec2 vectorErmineToWorld(float pixelX, float pixelY)
	{
		return vectorErmineToWorld(glm::vec2(pixelX, pixelY));
	}

	glm::vec2 vectorWorldToErmine(glm::vec2 world)
	{
		glm::vec2 PixelCoordinates;
		PixelCoordinates.x = scalarWorldToErmine(world.x);
		PixelCoordinates.y = scalarWorldToErmine(world.y);

		return PixelCoordinates;
	}
	glm::vec2 vectorWorldToErmine(float worldX,float worldY)
	{
		return vectorWorldToErmine(glm::vec2(worldX, worldY));
	}


	glm::vec2 vertexErmineToWorld(glm::vec2 screen)
	{
		auto WorldCoordinates = vectorErmineToWorld(screen);
		WorldCoordinates.y = WorldCoordinates.y * -1.0f;

		return WorldCoordinates;
	}
	glm::vec2 vertexErmineToWorld(float pixelX, float pixelY)
	{
		return vertexErmineToWorld(glm::vec2(pixelX, pixelY));
	}
	
	glm::vec2 vertexWorldToErmine(glm::vec2 world)
	{
		auto ErmineCoordinates = vectorWorldToErmine(world);
		ErmineCoordinates.y = ErmineCoordinates.y * -1.0f;

		return ErmineCoordinates;
	}
	glm::vec2 vertexWorldToErmine(float worldX, float worldY)
	{
		return vertexWorldToErmine(glm::vec2(worldX,worldY));
	}
}