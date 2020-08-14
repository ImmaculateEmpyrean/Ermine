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
		//Copy Constructor is deleted because two objects cannot share the position..
		PhysicsComponent2D(const PhysicsComponent2D& rhs) = delete;
		//Move Constructor Must Be The Staple Goto As The Copy Constructor Does Not Exist..
		PhysicsComponent2D(PhysicsComponent2D&& rhs);

		//Copy Assignment Operator is deleted because two objects cannot share the position..
		PhysicsComponent2D operator=(const PhysicsComponent2D& rhs) = delete;
		//Move Assignemnt Operator is the staple Goto As Copy Version Is Deleted..
		PhysicsComponent2D& operator=(PhysicsComponent2D&& rhs);

		//Use This Operator To Implicitly Convert This Physics Component To a B2Body.. Might Come in Handy..
		operator b2Body* () { return BodyManagedByTheComponent; }

		//Start Get Position And angle Of The Component In Space..//
		//Returns Location of The Body In Box2D World..
		glm::vec2 GetBodyLocationBox2DSpace();
		//Returns Location Of The Body In Ermine World..
		glm::vec2 GetBodyLocationPixelSpace();

		//Returns In Radians.... this is the radians rotated taking body centre into account..
		float GetAngleOfTheBody();
		//Ended Get Position And Angle Of The Component In Space..//

		//Start Get Various Matrices Related With Transformation//
		//Gives The Translation Matrix.. 
		glm::mat4 GetTranslationMatrix();
		//Gives The Rotation Matrix..
		glm::mat4 GetRotationMatrix();
		//Ended Get Various Matrices Related With Transformation//

	public:

	protected:

	protected:

	private:
		//void HelperCopyFunction(const PhysicsComponent2D& rhs);
		void HelperMoveFunction(PhysicsComponent2D&& rhs);

	private:
		b2Body* BodyManagedByTheComponent;
		b2BodyDef BodyDefinitionOfTheComponent;
		
		std::vector<b2FixtureDef> FixturesAssociatedWithTheBody;
	};
}