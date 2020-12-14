#pragma once
#include<iostream>
#include<vector>
#include<string>

#include "Physics/Physics.h"
#include "JointBase.h"

namespace Ermine
{
	class PhysicsComponent2D;
	class RubeLoader;

	class MotorJoint :public JointBase
	{
	protected:
		//There Is Absolutely No Point in An Empty Joint Existing..
		MotorJoint() = delete;

		//This Constructor Is To Be Used To Set Up Anchor Points All Coordinates In Pixel Space
		//MotorJoint(std::string JointName, b2Body* BodyA, b2Body* BodyB, bool CollideConnected = false); 

		/*
		*	MOTOR JOINT TO BE CONSTRUCTED FOR USING RUBE ONLY..
		*/

	private:
		//Constructor To Be Used Specially By Friends And No One Else..
		MotorJoint(b2Joint* Pointer, std::string JointName, b2Body* BodyA, b2Body* BodyB);

	public:
		//This Destructor Is An Override From JointBAse
		virtual ~MotorJoint() override;

		//Get The Concrete Type Of The Joint In Question..
		virtual Ermine::EJointType GetType() override { return Ermine::EJointType::MotorJoint; };

		//How Do You Even Copy A Joint ?
		MotorJoint(const MotorJoint& rhs) = delete;
		MotorJoint& operator=(const MotorJoint& rhs) = delete;

		//Well You Could Move A Joint
		MotorJoint(MotorJoint&& rhs);
		MotorJoint& operator=(MotorJoint&& rhs);

	private:
		static std::shared_ptr<Ermine::MotorJoint> Generate(b2Joint* Pointer, std::string JointName, b2Body* BodyA, b2Body* BodyB);

	public:
		glm::vec2 GetLinearOffset();
		void	  SetLinearOffset(glm::vec2 Offset);

		float GetAngularOffsetDegrees();
		float GetAngularOffsetRadians();
		void  SetAngularOffsetDegrees(float Offset);
		void  SetAngularOffsetRadians(float Offset);

		float GetMaxForce();
		void  SetMaxForce(float MaxForce);

		float GetMaxTorque();
		void  SetMaxTorque(float MaxTorque);

		float GetCorrectionFactor();
		void  SetCorrectionFactor(float CorrectionFactor);

	public:

	protected:

	protected:

	private:

	private:

		friend class Ermine::PhysicsComponent2D;
		friend class Ermine::RubeLoader;
	};
}