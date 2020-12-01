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

	//This Is Responsible For The Timestep By The Physics Engine
	extern float PhysicsWorldTimestep;

	//Variables For Instructing The Physics Engine
	extern int32 PhysicsVelocityIterations;
	extern int32 PhysicsPositionIterations;

	//Variables That Compute The Extent Of The Physics World..
	extern glm::vec2 PhysicsWorldTopLeft;
	extern glm::vec2 PhysicsWorldBottomRight;

	glm::vec2 B2Vec2ToGLM(b2Vec2 Vector);
	b2Vec2 GLMToB2Vec2(glm::vec2 Vector);

#pragma region AllTheseFunctionsAreWrong

	//Start Convert Box2D World Coordinates to Ermine Space Coordinates And Vice Versa//
	glm::vec2 coordWorldToErmine(glm::vec2 world);
	glm::vec2 coordErmineToWorld(glm::vec2 screen);
	//Ended Convert Box2D World Coordinates to Ermine Space Coordinates And Vice Versa//
	
	//Start Scale scalar quantity between worlds//
	float scalarErmineToWorld(float val);
	float scalarWorldToErmine(float val);
	//Ended Scale scalar quantity between worlds//

	//Start Scale vector between worlds//
	glm::vec2 vectorErmineToWorld(glm::vec2 screen);
	glm::vec2 vectorErmineToWorld(float pixelX, float pixelY);

	glm::vec2 vectorWorldToErmine(glm::vec2 world);
	glm::vec2 vectorWorldToErmine(float worldX,float worldY);
	//Ended Scale Vector Between Worlds

	//Start Functions To Specify Coordinates In PixelSpace For Model Vertexes.. (These Are Just Like Scalar Ermine To World And Vector ErmineToWorld Except That They Additionally Flip The Y axis So As To Make Stuff Consistent With Box2D..//
	glm::vec2 vertexErmineToWorld(glm::vec2 screen);
	glm::vec2 vertexErmineToWorld(float pixelX, float pixelY);

	glm::vec2 vertexWorldToErmine(glm::vec2 world);
	glm::vec2 vertexWorldToErmine(float worldX, float worldY);
	//Start Functions To Specify Coordinates In PixelSpace For Model Vertexes.. (These Are Just Like Scalar Ermine To World And Vector ErmineToWorld Except That They Additionally Flip The Y axis So As To Make Stuff Consistent With Box2D..//
#pragma endregion AllTheseFunctionsAreWrong
}