#pragma once
#include<iostream>
#include<vector>
#include<string>

#include <box2d/box2d.h>
#include "Physics.h"
#include "BodyPart.h"
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

		//Use This Constructor To Explicitly Fix The Size Of The Box And Also Say That The Shape Is a Box..
		PhysicsComponent2D(b2BodyDef Definition, b2FixtureDef FixtureDefinition,glm::vec2 BodySizeInPixelSpace);

		//Use This Constructor If U Wanna Set a Custom Shape..
		PhysicsComponent2D(b2BodyDef Definition, b2FixtureDef FixtureDefinition);

		//Use This Constructor If U Wanna Set A Lot Of Fixtures And Shapes..
		PhysicsComponent2D(b2BodyDef Definition,std::vector<BodyPart> AllPartsConstitutingTheBody);

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


		//Get Size Of The Body
		glm::vec2 GetBodySize() { return BodySize; }
		
		//Get Width Of The Body
		float GetBodyWidth() { return BodySize.x; }
		//Get Height of The Body
		float GetBodyHeight() { return BodySize.y; }
		//Ended Get Various Matrices Related With Transformation//

		//Start Section Add Something To The Body..//
		void AddForceToCentre(glm::vec2 Force);
		void AddForce(glm::vec2 Force,glm::vec2 BodyPointAtWhichForceIsApplied);

	public:

	protected:

	protected:

	private:
		void HelperConstructorConstructBody();
		void HelperMoveFunction(PhysicsComponent2D&& rhs);

		//Return The Width And Height Of The Bounding Box Of The Entire Box2D Object In Box2D Space.. REMEMBER THE BODY MUST EXIST FOR THIS TO WORK..
		glm::vec2 HelperGetWidthAndHeightOfTheBoundingBox();

	private:
		b2Body* BodyManagedByTheComponent;
		b2BodyDef BodyDefinitionOfTheComponent;
		
		std::vector<b2FixtureDef> FixturesAssociatedWithTheBody;

		glm::vec2 BodySize = glm::vec2(10.0f,5.0f);
	};
}