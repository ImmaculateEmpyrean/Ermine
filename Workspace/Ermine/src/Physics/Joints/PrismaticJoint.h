#pragma once
#include<iostream>
#include<vector>
#include<string>

#include "Physics/Physics.h"
#include "JointBase.h"

namespace Ermine
{
	//Forward Declaration So That They Can be Made Friends As We Implicitly Trust Them..
	class PhysicsComponent2D;
	class RubeLoader;

	class PrismaticJoint :public JointBase
	{
	protected:
		//There Is Ansolutely No Point In An Empty Joint Existing..
		PrismaticJoint() = delete;

		//This Constructor Is To Be Used To Set Up Sliding Axis Along With Reference Angle And Reference Points All Coordinates In Pixel Space
		PrismaticJoint(std::string JointName,b2Body* BodyA, b2Body* BodyB, glm::vec2 AnchorAWithRespectToBoxCentre, glm::vec2 AnchorBWithRespectToBoxCentre, float ReferenceAngleDegrees,glm::vec2 SlidingAxis, bool ShouldBodiesAttachedByTheJointCollide = false);

	private:
		//This Constructor Is Very Dangerous As It Assumes That The Pointer IS Constructed Properly..
		PrismaticJoint(b2Joint* Pointer,std::string JointName,b2Body* BodyA,b2Body* BodyB);

	public:
		//A Destructor Is Necessary As This Class Manages Memory..
		virtual ~PrismaticJoint() override;

		//Get The Concrete Type Of The Joint In Question..
		virtual Ermine::EJointType GetType() override { return Ermine::EJointType::PrismaticJoint; };

		PrismaticJoint(const PrismaticJoint& rhs) = delete;
		PrismaticJoint& operator=(const PrismaticJoint& rhs)=delete;

		PrismaticJoint(PrismaticJoint&& rhs);
		PrismaticJoint& operator=(PrismaticJoint&& rhs);

		static std::shared_ptr<Ermine::PrismaticJoint> Generate(std::string JointName, b2Body* BodyA, b2Body* BodyB, glm::vec2 AnchorAWithRespectToBoxCentre, glm::vec2 AnchorBWithRespectToBoxCentre, float ReferenceAngleDegrees, glm::vec2 SlidingAxis, bool ShouldBodiesAttachedByTheJointCollide = false);

	private:
		static std::shared_ptr<Ermine::PrismaticJoint> Generate(b2Joint* Pointer,std::string JointName, b2Body* BodyA, b2Body* BodyB);

	public:
		
		//Start Set And Manipulate Movement Limits Of The Joint In Question..//
		void EnableLimits	();
		void DisableLimits  ();
		void ToggleLimitFlag();
		
		glm::vec2 GetMovementLimits();
		float	  GetLowerLimitMovement();
		float	  GetUpperLimitMovement();

		//Start Set Movement Limits Of The Prismatic Joint In Question..//
		void SetMovementLimits(glm::vec2 Limits);
		void SetMovementLimits(float LowerLimitMovement, float UpperLimitMovement);
		
		void SetMovementLimitsUpper(float UpperLimit);
		void SetMovementLimitsLower(float LowerLimit);
		//Ended Set Movement Limits Of The Prismatic Joint In Question..//

		//Ended Set And Manipulate Movement Limit Of The Joints In Question..//

		//Start Query Abt Anchor Points..
		glm::vec2 GetBodyALocalAnchorLocation();
		glm::vec2 GetBodyBLocalAnchorLocation();
		//Ended Query Abt Anchor Points..

		//Start This Joint Incorporates A Motor So Basic Motor Functions Are Provided//

		//Start Motor Functions.. Interact With Motor Only In Newtons
		bool  IsMotorEnabled();
		
		void  EnableMotor(bool enabled);
		void  SetMotorSpeed(float speed);
		float GetMotorSpeed();
		
		//float GetMaxMotorSpeed();
		
		void  SetMaxMotorForce(float Force);
		float GetMotorForce();
		//Ended Motor Functions.. Interact With Motor Only In Newtons


		//Start Get Reference Angle Functions//
		float GetReferenceAngleInDegrees();
		float GetReferenceAngleInRadians();
		//Ended Get Reference Angle Functions//

	public:

	protected:

	protected:

	private:
		
	private:
		friend class Ermine::RubeLoader;
		friend class Ermine::PhysicsComponent2D;
	};
}
