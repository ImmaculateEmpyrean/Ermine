#pragma once
#include<iostream>
#include<vector>
#include<string>

#include <box2d/box2d.h>
#include "Physics.h"
#include "BodyPart.h"

#include "Joints/JointBase.h"
#include "Joints/DistanceJoint.h"
#include "Joints/RevoluteJoint.h"

/*
	This Component Must Be Inherited By Any Class In Ermine If It Wants Physics Implemented..	
*/

namespace Ermine
{
	//This Class Is Forward Declared So That It Can Be Made A Friend.. We Need To Access This Class Inorder To Draw The Outlines On Screen In The Debug Mode..
	class Renderer2D;

	class PhysicsComponent2D
	{
	public:
		//A Default Physics component Can Be Constructed However It Cannot Be Default..
		PhysicsComponent2D();

		//Use This Constructor To Quickly Construct A Static Box At THe Specified Location No Other Nonsense For Now..
		PhysicsComponent2D(glm::vec2 BodyLocationInPixelSpace, glm::vec2 BodySizeInPixelSpace ,bool StaticBody = true);

		//Use This Constructor To Explicitly Fix The Size Of The Box And Also Say That The Shape Is a Box..
		PhysicsComponent2D(b2BodyDef Definition, b2FixtureDef FixtureDefinition,glm::vec2 BodySizeInPixelSpace);

		//Use This Constructor If U Wanna Set a Custom Shape..
		PhysicsComponent2D(b2BodyDef Definition, b2FixtureDef FixtureDefinition);

		//Use This Constructor If U Wanna Set A Lot Of Fixtures And Shapes..
		PhysicsComponent2D(b2BodyDef Definition,std::vector<b2FixtureDef> FixtureDefinitions);

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

		//Start Get Size Of The Body//

		//Get Size Of The Body Box2D Space
		glm::vec2 GetBodySizeBox2DSpace() { return BodySize; }
		//Get Size Of The Body Pixel Space
		glm::vec2 GetBodySizePixelSpace() { return Ermine::vectorWorldToPixels(BodySize); }
		
		//Get Width Of The Body In Box2D Space
		float GetBodyWidthBox2DSpace() { return BodySize.x; }
		//Get Width Of The Body In Pixel Space
		float GetBodyWidthPixelSpace() { return Ermine::scalarWorldToPixels(BodySize.x); }

		//Get Height of The Body In Box2D Space
		float GetBodyHeightBox2DSpace() { return BodySize.y; }
		//Get Height of The Body in Pixel Space
		float GetBodyHeightPixelSpace() { return Ermine::scalarWorldToPixels(BodySize.y);}
		
		//Ended Get Size Of The Body//

		//Start Section Add Something To The Body..//
		void AddForceToCentre(glm::vec2 Force);
		void AddForce(glm::vec2 Force,glm::vec2 BodyPointAtWhichForceIsApplied);
		//Ended Section Add Something To The Body..//

		//Start Section Utility Functions..//

		//Attaches This Component To The Debugger Which Traces All Its Shapes In Realtime So That Debugging Physics Is Made Easier..
		void StartDebugTrace();
		
		//Detaches This Component From The Debugger Which Traces All Its Shapes In Realtime.
		void StopDebugTrace();

		//Ended Section Utility Functions..//

		//Start Creating Joint Functions..//

		//This Is The Best Way To Delete A Joint Just Give It The Identifier You Wanna Delete..
		void DeleteJoint(unsigned int Identifier);

		//Start Creating Distance Joint..// 
		JointBase* CreateDistanceJoint(PhysicsComponent2D* BodyB,bool CollideCollision=false);
		JointBase* CreateDistanceJoint(PhysicsComponent2D* BodyB, glm::vec2 LocalAnchorPointA, bool CollideCollision = false);
		JointBase* CreateDistanceJoint(PhysicsComponent2D* BodyB, glm::vec2 LocalAnchorPointA, glm::vec2 LocalAnchorPointB, bool CollideCollision = false);
		//Ended Creating Distance Joint..//

		//Start Creating Revolute Joint..//
		JointBase* CreateRevoluteJoint(PhysicsComponent2D* BodyB, bool CollideCollision = false);
		//Ended Creating Revolute Joint..//

		//Ended Creating Joint Functions..//
	public:

	protected:

	protected:

	private:
		void HelperConstructorConstructBody();
		void HelperMoveFunction(PhysicsComponent2D&& rhs);

		//Return The Width And Height Of The Bounding Box Of The Entire Box2D Object In Box2D Space.. REMEMBER THE BODY MUST EXIST FOR THIS TO WORK..
		glm::vec2 HelperGetWidthAndHeightOfTheBoundingBox();

	private:
		//This Handle Is Stored Inside The Physics Component Class So That We Can Submit The Body To The Renderer Easily..
		static std::function<void(PhysicsComponent2D*)> FuncSubmitBodyToRenderer2D;

		//This Handle Is Stored Inside The Physics Component Class So That We Can Detach The Body From The Renderer Easily..
		static std::function<void(PhysicsComponent2D*)> FuncDetachBodyFromRenderer2D;

		bool IsDebugTraceEnabled = false;

		b2Body* BodyManagedByTheComponent;
		b2BodyDef BodyDefinitionOfTheComponent;
		
		std::vector<b2FixtureDef> FixturesAssociatedWithTheBody;

		std::unordered_map<unsigned int, Ermine::JointBase*> JointsBuffer;
		//std::vector<Ermine::JointBase*> JointsBuffer;

		glm::vec2 BodySize = glm::vec2(10.0f,5.0f);

		friend class Ermine::Renderer2D;
	};
}