#pragma once
#include<iostream>
#include<vector>
#include<string>

#include <box2d/box2d.h>
#include "Physics.h"
#include "BodyPart.h"
#include "Physics/FixtureUserDataStruct.h"

#include "Joints/JointBase.h"
#include "Joints/DistanceJoint.h"
#include "Joints/RevoluteJoint.h"
#include "Joints/PrismaticJoint.h"
#include "Joints/WheelJoint.h"
#include "Joints/RopeJoint.h"
#include "Joints/WeldJoint.h"

/*
	This Component Must Be Inherited By Any Class In Ermine If It Wants Physics Implemented..	
*/

namespace Ermine
{
	//This Class Is Forward Declared So That It Can Be Made A Friend.. We Need To Access This Class Inorder To Draw The Outlines On Screen In The Debug Mode..
	class Renderer2D;
	class PhysicsActor2D;

	class PhysicsComponent2D
	{
#pragma region Constructors

	protected:
		//Use This Constructor If U Wanna Set A Lot Of Fixtures And Shapes..
		PhysicsComponent2D(b2BodyDef Definition,std::vector<b2FixtureDef> FixtureDefinitions);

		//A Destructor Is Needed As This Class Is Managing Memory
		~PhysicsComponent2D();

	public:
		//Copy Constructor is disabled BEWARE because two objects cannot share the position..
		PhysicsComponent2D(const PhysicsComponent2D& rhs) = delete;
		//Move Constructor Must Be The Staple Goto As The Copy Constructor Does Not Exist..
		PhysicsComponent2D(PhysicsComponent2D&& rhs);

		//Copy Assignment Operator is disabled BEWARE because two objects cannot share the position..
		PhysicsComponent2D& operator=(const PhysicsComponent2D& rhs) = delete;
		//Move Assignemnt Operator is the staple Goto As Copy Version Is Deleted..
		PhysicsComponent2D& operator=(PhysicsComponent2D&& rhs);
#pragma endregion

#pragma region Generators

	public:
		static std::shared_ptr<Ermine::PhysicsComponent2D> Generate(b2BodyDef BodyDefinition, std::vector<b2FixtureDef> FixturesAssociatedWithBody);
#pragma endregion

		//Use This Operator To Implicitly Convert This Physics Component To a B2Body.. Might Come in Handy..
		operator b2Body* () { return BodyManagedByTheComponent; }

		//Start Get Position And angle Of The Component In Space..//
		glm::vec2 GetBodyLocationBox2DSpace();
		glm::vec2 GetBodyLocationPixelSpace();

		float GetAngleOfTheBodyRadians();
		float GetAnfleOfTheBodyDegrees();
		//Ended Get Position And Angle Of The Component In Space..//

		//Start Get Various Matrices Related With Transformation//
		glm::mat4 GetModelMatrix();
		glm::mat4 GetTranslationMatrix();
		glm::mat4 GetRotationMatrix();
		//Ended Get Various Matrices Related With Transformation//

		//Start Get Some Value From The Body
		glm::vec2 GetVelocityOfTheBody();
		float GetAngularVelocityOfTheBody();
		//Ended Get Some Value From The Body

		//Start Section Add Something To The Body..//
		void AddForceToCentre(glm::vec2 Force);
		void AddForce(glm::vec2 Force,glm::vec2 WorldPositionInPixelCoordinatesWhereTheForceWasApplied);
		//Ended Section Add Something To The Body..//

		//Start Section Set Something To The Body..//
		void SetPosition(glm::vec2 Position);
		
		void SetVelocity(glm::vec2 Velocity);
		void SetAngularVelocity(float AngularVelocity);
		//Ended Section Set Something To The Body..//

#pragma region Joints
		
		std::shared_ptr<JointBase> GetJoint	 (std::string JointIdentificationName);
		void					 DestroyJoint(std::string JointIdentificationName);

		//Start Creating Distance Joint..// 
		std::shared_ptr<Ermine::JointBase> CreateDistanceJoint(std::string JointName,PhysicsComponent2D* BodyB,bool CollideCollision = false);
		std::shared_ptr<Ermine::JointBase> CreateDistanceJoint(std::string JointName,PhysicsComponent2D* BodyB, glm::vec2 LocalAnchorPointA, bool CollideCollision = false);
		std::shared_ptr<Ermine::JointBase> CreateDistanceJoint(std::string JointName,PhysicsComponent2D* BodyB, glm::vec2 LocalAnchorPointA, glm::vec2 LocalAnchorPointB, bool CollideCollision = false);
		//Ended Creating Distance Joint..//

		//Start Creating Revolute Joint..//
		std::shared_ptr<Ermine::JointBase> CreateRevoluteJoint(std::string JointName,PhysicsComponent2D* BodyB, bool CollideCollision = false);
		std::shared_ptr<Ermine::JointBase> CreateRevoluteJoint(std::string JointName,PhysicsComponent2D* BodyB, glm::vec2 LocalAnchorPointA, bool CollideCollision = false);
		std::shared_ptr<Ermine::JointBase> CreateRevoluteJoint(std::string JointName,PhysicsComponent2D* BodyB, glm::vec2 LocalAnchorPointA, glm::vec2 LocalAnchorPointB, bool CollideCollision = false);
		std::shared_ptr<Ermine::JointBase> CreateRevoluteJoint(std::string JointName,PhysicsComponent2D* BodyB, glm::vec2 LocalAnchorPointA, glm::vec2 LocalAnchorPointB,float ReferenceAngleInDegrees, bool CollideCollision = false);
		//Ended Creating Revolute Joint..//

		//Start Creating Prismatic Joint..//
		std::shared_ptr<Ermine::JointBase> CreatePrismaticJoint(std::string JointName,PhysicsComponent2D* BodyB, bool CollideConnected = false);
		std::shared_ptr<Ermine::JointBase> CreatePrismaticJoint(std::string JointName,PhysicsComponent2D* BodyB, glm::vec2 LocalAnchorPointA, bool CollideCollision = false);
		std::shared_ptr<Ermine::JointBase> CreatePrismaticJoint(std::string JointName,PhysicsComponent2D* BodyB, glm::vec2 LocalAnchorPointA, glm::vec2 LocalAnchorPointB, bool CollideCollision = false);
		std::shared_ptr<Ermine::JointBase> CreatePrismaticJoint(std::string JointName,PhysicsComponent2D* BodyB, glm::vec2 LocalAnchorPointA, glm::vec2 LocalAnchorPointB, float ReferenceAngleInRadians, bool CollideCollision = false);
		std::shared_ptr<Ermine::JointBase> CreatePrismaticJoint(std::string JointName,PhysicsComponent2D* BodyB, glm::vec2 LocalAnchorPointA, glm::vec2 LocalAnchorPointB, float ReferenceAngleInRadians,glm::vec2 SlidingAxis, bool CollideCollision = false);
		//Ended Creating Prismatic Joint..//

		//Start Creating Wheel Joint..//
		std::shared_ptr<Ermine::JointBase> CreateWheelJoint(std::string JointName ,PhysicsComponent2D* BodyB, bool CollideConnected = false);
		//Ended Creating Wheel Joint..//

		//Start Creating Rope Joint..//
		std::shared_ptr<Ermine::JointBase> CreateRopeJoint(std::string JointName,PhysicsComponent2D* BodyB, bool CollideConnected = false);
		std::shared_ptr<Ermine::JointBase> CreateRopeJoint(std::string JointName,PhysicsComponent2D* BodyB, float RopeLength, bool CollideConnected = false);
		std::shared_ptr<Ermine::JointBase> CreateRopeJoint(std::string JointName,PhysicsComponent2D* BodyB, glm::vec2 LocalAnchorAPixelCoordinates, glm::vec2 LocalAnchorBPixelCoordinates,float RopeLength, bool CollideConnected = false);
		//Ended Creating Rope Joint..//

		//Start Creating Weld Joint..//
		std::shared_ptr<Ermine::JointBase> CreateWeldJoint(std::string JointName,PhysicsComponent2D* BodyB, bool CollideConnected = false);
		std::shared_ptr<Ermine::JointBase> CreateWeldJoint(std::string JointName,PhysicsComponent2D* BodyB, glm::vec2 LocalAnchorAPixelCoordinates, bool CollideConnected = false);
		std::shared_ptr<Ermine::JointBase> CreateWeldJoint(std::string JointName,PhysicsComponent2D* BodyB, glm::vec2 LocalAnchorAPixelCoordinates, glm::vec2 LocalAnchorBPixelCoordinates, bool CollideConnected = false);
		//Ended Creating Joint Functions..//
#pragma endregion
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

		//The Shared Pointer Will Hopefully Delete The Joint Properly Once The Two Bodies In Question Are Destroyed.. No Need To Manually Handle Joints In This Approach
		std::vector<std::shared_ptr<JointBase>> JointsBuffer;

		friend class Ermine::Renderer2D;
		friend class Ermine::PhysicsActor2D;
	};
}