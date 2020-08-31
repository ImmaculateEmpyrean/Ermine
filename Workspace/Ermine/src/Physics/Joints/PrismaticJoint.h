#pragma once
#include<iostream>
#include<vector>
#include<string>

#include "Physics/Physics.h"
#include "JointBase.h"

namespace Ermine
{
	class PrismaticJoint :public JointBase
	{
	public:
		//There Is Ansolutely No Point In An Empty Joint Existing..
		PrismaticJoint() = delete;

		//This Is More Or Less The Default Constructir You Are Expected To Use..
		PrismaticJoint(b2Body* BodyA, b2Body* BodyB, bool ShouldBodiesAttachedByTheJointCollide = false);

		//This Constructor Is To Be Used To Set Up Anchor Points All Coordinates In Pixel Space
		PrismaticJoint(b2Body* BodyA, b2Body* BodyB, glm::vec2 AnchorAWithRespectToBoxCentre, bool ShouldBodiesAttachedByTheJointCollide = false);

		//This Constructor Is To Be Used To Set Up Anchor Points All Coordinates In Pixel Space
		PrismaticJoint(b2Body* BodyA, b2Body* BodyB, glm::vec2 AnchorAWithRespectToBoxCentre, glm::vec2 AnchorBWithRespectToBoxCentre, bool ShouldBodiesAttachedByTheJointCollide = false);

		//This Constructor Is To Be Used To Set Up Reference Angle And Anchor Points All Coordinates In Pixel Space
		PrismaticJoint(b2Body* BodyA, b2Body* BodyB, glm::vec2 AnchorAWithRespectToBoxCentre, glm::vec2 AnchorBWithRespectToBoxCentre,float ReferenceAngleDegrees, bool ShouldBodiesAttachedByTheJointCollide = false);

		//This Constructor Is To Be Used To Set Up Sliding Axis Along With Reference Angle And Reference Points All Coordinates In Pixel Space
		PrismaticJoint(b2Body* BodyA, b2Body* BodyB, glm::vec2 AnchorAWithRespectToBoxCentre, glm::vec2 AnchorBWithRespectToBoxCentre, float ReferenceAngleDegrees,glm::vec2 SlidingAxis, bool ShouldBodiesAttachedByTheJointCollide = false);

		//A Destructor Is Necessary As This Class Manages Memory..
		virtual ~PrismaticJoint() override;

	public:
		virtual b2Joint* GetJoint() override { return PrismaticJointHandle; }
		virtual operator b2Joint* () override { return PrismaticJointHandle; }

		//Set Movement Limits Is Used To Set Up How Much Movement Is Allowed By The Bodies Connected By The Joints..
		void SetMovementLimits(float LowerLimitMovement, float UpperLimitMovement);
		void SetMovementLimits(float LimitMovement, bool IsUpperLimit);
		void ClearLimits();

		//Start Query Abt Anchor Points..
		
		//You Will Get It Still In Pixel Coordinates 50 or something Maybe not 0.5.. 
		glm::vec2 GetAnchorALocLocalSpace();
		glm::vec2 GetAnchorBLocLocalSpace();

		//It Does Say Pixel Coordinate System Right.. 
		glm::vec2 GetAnchorALocPixelSpace();
		glm::vec2 GetAnchorBLocPixelSpace();

		//Ended Query Abt Anchor Points..

		//Start This Joint Incorporates A Motor So Basic Motor Functions Are Provided//

		//alter joint motor

		void EnableMotor(bool enabled);
		void SetMotorSpeed(float speed);
		//Max Motor Force To Be Set In Newtons
		void SetMaxMotorForce(float Force);

		//query joint motor

		bool IsMotorEnabled();
		float GetMotorSpeed();
		float GetMotorForce();

		//Ended This Joint Incorporates A Motor So Basic Motor Functions Are Provided//

		//Start Get Reference Angle Functions//

		float GetReferenceAngleInDegrees();
		float GetReferenceAngleInRadians();

		//Ended Get Reference Angle Functions//

	public:

	protected:

	protected:

	private:
		void HelperConstructPrismaticJoint(b2Body* BodyA, b2Body* BodyB, glm::vec2 AnchorAWithRespectToBoxCentre, glm::vec2 AnchorBWithRespectToBoxCentre,
			glm::vec2 SlidingAxis, float ReferenceAngleDegrees , bool ShouldBodiesAttachedByTheJointCollide);

	private:
		b2PrismaticJoint* PrismaticJointHandle = nullptr;
	};
}
