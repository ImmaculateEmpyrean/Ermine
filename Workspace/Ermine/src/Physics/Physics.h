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

	//ReCalculate Physics World Bounds Using This Function..
	///In The Future See That This Function Is Called When ScreenHeight Or ScreenWidth Is Changed
	void RecalculatePhysicsWorldBounds();

	glm::vec2 B2Vec2ToGLM(b2Vec2 Vector);
	b2Vec2 GLMToB2Vec2(glm::vec2 Vector);

#pragma region AllTheseFunctionsAreWrong

	//Start Convert Box2D World Coordinates to Pixel Space Coordinates//
	glm::vec2 coordWorldToPixels(glm::vec2 world);
	//glm::vec2 coordWorldToPixels(float worldX, float worldY);
	//Ended Convert Box2D World Coordinates to Pixel Space Coordinates//

	///Think I Fixed The Following BAtch But Still Need More Testing to be Done..
	//Start Convert Pixel Space Coordinates to Box2D World Coordinates// 
	glm::vec2 coordPixelsToWorld(glm::vec2 screen); 
	//glm::vec2 coordPixelsToWorld(float pixelX, float pixelY);
	//Ended Convert Pixel Space Coordinates to Box2D World Coordinates//
	///EndCard.. for the batch

	//Start Scale scalar quantity between worlds//
	float scalarPixelsToWorld(float val);
	float scalarWorldToPixels(float val);
	//Ended Scale scalar quantity between worlds//

	//Start Scale vector between worlds//
	glm::vec2 vectorPixelsToWorld(glm::vec2 screen);
	glm::vec2 vectorPixelsToWorld(float pixelX, float pixelY);

	glm::vec2 vectorWorldToPixels(glm::vec2 world);
	glm::vec2 vectorWorldToPixels(float worldX,float worldY);
	//Ended Scale Vector Between Worlds

	//Start Function Which Are Use To Set Location Of The Vertexes Of Physics Objects//
	glm::vec2 vertexPixelsToWorld(glm::vec2 screen);
	glm::vec2 vertexPixelsToWorld(float pixelX, float pixelY);

	glm::vec2 vertexWorldToPixels(glm::vec2 world);
	glm::vec2 vertexWorldToPixels(float worldX, float worldY);
	//Ended Function Which Are Use To Set Location Of The Vertexes Of Physics Objects//
#pragma endregion AllTheseFunctionsAreWrong
}