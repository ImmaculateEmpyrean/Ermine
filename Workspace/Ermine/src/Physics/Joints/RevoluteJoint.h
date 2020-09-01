#pragma once
#include<iostream>
#include<vector>
#include<string>

#include "Physics/Physics.h"
#include "JointBase.h"

namespace Ermine
{
	class RevoluteJoint : public JointBase
	{
	public:
		//No Point In Having A Default Revolute Joint.. The Joint Must Hold Bodies..
		RevoluteJoint() = delete;

		//This Is Something Like The Default Constructor In This Case Use It Most Of The Time
		RevoluteJoint(b2Body* BodyA, b2Body* BodyB, bool ShouldBodiesAttachedByTheJointCollide = false);

		//This Constructor Is To Be Used To Set Up Anchor Points All Coordinates In Pixel Space
		RevoluteJoint(b2Body* BodyA, b2Body* BodyB, glm::vec2 AnchorAWithRespectToBoxCentre, bool ShouldBodiesAttachedByTheJointCollide = false);

		//This Constructor Is To Be Used To Set Up Anchor Points All Coordinates In Pixel Space
		RevoluteJoint(b2Body* BodyA, b2Body* BodyB, glm::vec2 AnchorAWithRespectToBoxCentre, glm::vec2 AnchorBWithRespectToBoxCentre, bool ShouldBodiesAttachedByTheJointCollide = false);

		//This Constructor Is To Be Used To Set Up Anchor Points All Coordinates In Pixel Space And Also Reference Angle While At It.. Use This Constructor If You Are Gonna Call Get Reference Function In The Future..
		RevoluteJoint(b2Body* BodyA, b2Body* BodyB, glm::vec2 AnchorAWithRespectToBoxCentre, glm::vec2 AnchorBWithRespectToBoxCentre,float ReferenceAngleDegrees, bool ShouldBodiesAttachedByTheJointCollide = false);

		//Destructor To Get Rid Of The Joint After It Is Used Up..
		virtual ~RevoluteJoint() override;

	public:
		virtual b2Joint* GetJoint() override { return RevoluteJointHandle; }
		virtual operator b2Joint* () override { return RevoluteJointHandle; }

		//Set Rotation Limits Is Used To Set Up How Much Relative Rotation Is Allowed Between Bodies Connected By The Joints..
		void SetRotationLimits(float LowerLimitDegrees,float UpperLimitDegrees);
		void SetRotationLimits(float LimitDegrees, bool IsUpperLimit);
		void ClearLimits();
		//Start This Joint Incorporates A Motor So Basic Motor Functions Are Provided//
		
		//Start Query Abt Anchor Points..

		//You Will Get It Still In Pixel Coordinates 50 or something Maybe not 0.5.. 
		virtual glm::vec2 GetBodyALocalAnchorLocation() override;
		virtual glm::vec2 GetBodyBLocalAnchorLocation() override;

		virtual glm::vec2 GetBodyAWorldAnchorLocationPixels() override;
		virtual glm::vec2 GetBodyBWorldAnchorLocationPixels() override;

		//Ended Query Abt Anchor Points..

		//alter joint motor
		
		void EnableMotor(bool enabled);
		void SetMotorSpeed(float speed);
		void SetMaxMotorTorque(float torque);

		//query joint motor
		
		bool IsMotorEnabled();
		float GetMotorSpeed();
		float GetMotorTorque();
		
		//Ended This Joint Incorporates A Motor So Basic Motor Functions Are Provided//

		//Start Get Reference Angle Functions//

		float GetReferenceAngleInDegrees();
		float GetReferenceAngleInRadians();

		//Ended Get Reference Angle Functions//

	public:

	protected:

	protected:

	private:
		void HelperConstructRevoluteJoint(b2Body* BodyA, b2Body* BodyB, glm::vec2 AnchorAWithRespectToBoxCentre, glm::vec2 AnchorBWithRespectToBoxCentre, float ReferenceAngle, bool ShouldBodiesAttachedByTheJointCollide);
		
	private:
		b2RevoluteJoint* RevoluteJointHandle = nullptr;

	};
}