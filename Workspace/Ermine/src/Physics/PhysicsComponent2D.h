#pragma once
#include<iostream>
#include<vector>
#include<string>

#include <box2d/box2d.h>
#include "Physics.h"
/*
	This Component Must Be Inherited By Any Class In Ermine If It Wants Physics Implemented..	
*/

namespace Ermine
{
	struct PhysicsComponentShape
	{
		b2PolygonShape Shape; //this is the shape which is fixed to fixture..
		b2FixtureDef Fixture; //this is the fixture that is bound to the body in the physics component
		glm::vec2 OffsetFromBodyCentre; //this is the offset of the centre of the shape from the centre of the body..
	};

	class PhysicsComponent2D
	{
	public:
		//A Default Physics component Can Be Constructed However It Cannot Be Default..
		PhysicsComponent2D();

		//Use This Constructor For More Of Your Work As Compared To The Default One..
		//This Fixes The Shape As A SingleBox.. To Have Something Else Go With A Different Constructor
		PhysicsComponent2D(b2BodyDef Definition, b2FixtureDef FixtureDefinition);

		//A Destructor Is Needed As This Class Is Managing Memory
		~PhysicsComponent2D();
	public:
		//Copy Constructor Must Be Overloaded As This Class Is Managing Pointers
		PhysicsComponent2D(const PhysicsComponent2D& rhs);
		//Move Constructor May as well Be Overloaded As We Are Overloading Copy Constructor
		PhysicsComponent2D(PhysicsComponent2D&& rhs);

		//Copy Assignment Operator Must Be Overloaded As This Class Is Managing Pointers
		PhysicsComponent2D operator=(const PhysicsComponent2D& rhs);
		//Move Assignemnt Operator May as well Be Overloaded As We Are Overloading Copy Constructor
		PhysicsComponent2D operator=(PhysicsComponent2D&& rhs);

		//Use This Operator To Implicitly Convert This Physics Component To a B2Body.. Might Come in Handy..
		operator b2Body* () { return BodyManagedByTheComponent; }

	public:

	protected:

	protected:

	private:
		void HelperCopyFunction(const PhysicsComponent2D& rhs);
		void HelperMoveFunction(PhysicsComponent2D&& rhs);

	private:
		b2Body* BodyManagedByTheComponent;
		b2BodyDef BodyDefinitionOfTheComponent;
		
		std::vector<PhysicsComponentShape> FixturesAssociatedWithTheBody;
	};
}