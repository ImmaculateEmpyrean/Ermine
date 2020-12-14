#include "stdafx.h"
#include "MotorJoint.h"

namespace Ermine
{
	MotorJoint::MotorJoint(b2Joint* Pointer, std::string JointName, b2Body* BodyA, b2Body* BodyB)
		:
		JointBase(JointName,BodyA,BodyB)
	{
		//This Is Why This Constructor Is Dangerous.. You Have To Trus That The File Was Properly Constructed..
		JointHandle = Pointer;
	}

	MotorJoint::~MotorJoint()
	{}


	MotorJoint::MotorJoint(MotorJoint&& rhs)
		:
		JointBase(std::move(rhs))
	{}
	MotorJoint& MotorJoint::operator=(MotorJoint&& rhs)
	{
		JointBase::operator=(std::move(rhs));
		return *this;
	}


	std::shared_ptr<Ermine::MotorJoint> MotorJoint::Generate(b2Joint* Pointer, std::string JointName, b2Body* BodyA, b2Body* BodyB)
	{
		return std::shared_ptr<Ermine::MotorJoint>(new MotorJoint(Pointer, JointName, BodyA, BodyB));
	}


	glm::vec2 MotorJoint::GetLinearOffset()
	{
		b2Vec2 LinearOffset = ((b2MotorJoint*)JointHandle)->GetLinearOffset();
		glm::vec2 LinearOffsetInErmine = Ermine::vectorWorldToErmine(Ermine::B2Vec2ToGLM(LinearOffset));

		return LinearOffsetInErmine;
	}
	void MotorJoint::SetLinearOffset(glm::vec2 Offset)
	{
		b2Vec2 LinearOffsetInWorld = Ermine::GLMToB2Vec2(Ermine::vectorErmineToWorld(Offset));
		((b2MotorJoint*)JointHandle)->SetLinearOffset(LinearOffsetInWorld);
	}

	float MotorJoint::GetAngularOffsetDegrees()
	{
		return glm::degrees<float>(GetAngularOffsetRadians());
	}
	float MotorJoint::GetAngularOffsetRadians()
	{
		return ((b2MotorJoint*)JointHandle)->GetAngularOffset();
	}
	void MotorJoint::SetAngularOffsetDegrees(float Offset)
	{
		SetAngularOffsetRadians(glm::radians<float>(Offset));
	}
	void MotorJoint::SetAngularOffsetRadians(float Offset)
	{
		((b2MotorJoint*)JointHandle)->SetAngularOffset(Offset);
	}

	float MotorJoint::GetMaxForce()
	{
		//Maybe We Have To Convert This.. Check In The Future..
		return ((b2MotorJoint*)JointHandle)->GetMaxForce();
	}
	void MotorJoint::SetMaxForce(float MaxForce)
	{
		((b2MotorJoint*)JointHandle)->SetMaxForce(MaxForce);
	}

	float MotorJoint::GetMaxTorque()
	{
		//Maybe We Have To Convert This.. Check In The Future..
		return ((b2MotorJoint*)JointHandle)->GetMaxTorque();
	}
	void MotorJoint::SetMaxTorque(float MaxTorque)
	{
		((b2MotorJoint*)JointHandle)->SetMaxTorque(MaxTorque);
	}

	float MotorJoint::GetCorrectionFactor()
	{
		return ((b2MotorJoint*)JointHandle)->GetCorrectionFactor();
	}
	void MotorJoint::SetCorrectionFactor(float CorrectionFactor)
	{
		((b2MotorJoint*)JointHandle)->SetCorrectionFactor(CorrectionFactor);
	}
}