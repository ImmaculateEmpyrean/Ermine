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
#pragma region Constructors

	protected:
		//No Point In Having A Default Revolute Joint.. The Joint Must Hold Bodies..
		RevoluteJoint() = delete;

		//This Constructor Is To Be Used To Set Up Anchor Points All Coordinates In Pixel Space And Also Reference Angle While At It.. Use This Constructor If You Are Gonna Call Get Reference Function In The Future..
		RevoluteJoint(std::string JointName,b2Body* BodyA, b2Body* BodyB, glm::vec2 AnchorAWithRespectToBoxCentre, glm::vec2 AnchorBWithRespectToBoxCentre,float ReferenceAngleDegrees, bool ShouldBodiesAttachedByTheJointCollide = false);

	public:
		//Destructor To Get Rid Of The Joint After It Is Used Up..
		virtual ~RevoluteJoint() override;

		//Get The Concrete Type Of The Joint In Question..
		virtual Ermine::EJointType GetType() override { return Ermine::EJointType::RevoluteJoint; };

		RevoluteJoint(const RevoluteJoint& rhs) = delete;
		RevoluteJoint& operator=(const RevoluteJoint& rhs) = delete;

		RevoluteJoint(RevoluteJoint&& rhs);
		RevoluteJoint& operator=(RevoluteJoint&& rhs);
#pragma endregion

		static std::shared_ptr<Ermine::RevoluteJoint> Generate(std::string JointName,b2Body* BodyA, b2Body* BodyB, glm::vec2 AnchorAWithRespectToBoxCentre, glm::vec2 AnchorBWithRespectToBoxCentre, float ReferenceAngleDegrees, bool ShouldBodiesAttachedByTheJointCollide = false);

	public:
		//Set Rotation Limits Is Used To Set Up How Much Relative Rotation Is Allowed Between Bodies Connected By The Joints..
		bool LimitEnabled();
		
		void EnableLimit();
		void DisableLimit();
		void ToggleJointLimits ();

		glm::vec2 GetRotationLimitsDegrees();
		glm::vec2 GetRotationLimitsRadians();
		void SetRotationLimits(float LowerLimit,float UpperLimit,bool Degrees);

		float GetRotationLimitsUpperDegrees();
		float GetRotationLimitsUpperRadians();
		void SetRotationLimitsUpper(float Angle, bool Degrees = true);

		float GetRotationLimitsLowerDegrees();
		float GetRotationLimitsLowerRadians();
		void  SetRotationLimitsLower(float Angle, bool Degrees = true);

		//Start This Joint Incorporates A Motor So Basic Motor Functions Are Provided//
		
		//Start Query Abt Anchor Points..

		//You Will Get It Still In Pixel Coordinates 50 or something Maybe not 0.5.. 
		virtual glm::vec2 GetBodyALocalAnchorLocation() override;
		virtual glm::vec2 GetBodyBLocalAnchorLocation() override;
		//Ended Query Abt Anchor Points..

		//Start Query Abt Motor State//

		//alter joint motor
		void EnableMotor(bool enabled);
		void SetMotorSpeed(float speed);
		void SetMaxMotorTorque(float torque);
		//query joint motor
		bool  IsMotorEnabled();
		float GetMotorSpeed();
		float GetMotorTorque();
		//Ended Query Abt Motor State//

		//Start Get Joint Speed Functions//
		float GetJointSpeedDegreesPerSecond();
		float GetJointSpeedRadiansPerSecond();
		//Ended Get Joint Speed Functions//

		//Start Get Reference Angle Functions//
		float GetReferenceAngleInDegrees();
		float GetReferenceAngleInRadians();
		//Ended Get Reference Angle Functions//

		//Start Get Joint Angle//
		float GetJointAngleInDegrees();
		float GetJointAngleInRadians();
		//Ended Get Join Angle//

	public:

	protected:

	protected:

	private:
		
		
	private:


	};
}