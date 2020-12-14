#include "stdafx.h"
#include "RevoluteJoint.h"

Ermine::RevoluteJoint::RevoluteJoint(std::string JointName,b2Body* BodyA, b2Body* BodyB, glm::vec2 AnchorAWithRespectToBoxCentre, glm::vec2 AnchorBWithRespectToBoxCentre, float ReferenceAngleDegrees, bool ShouldBodiesAttachedByTheJointCollide)
	:
	JointBase(JointName,BodyA, BodyB)
{
	b2Vec2 AnchorA = Ermine::GLMToB2Vec2(Ermine::vertexErmineToWorld(AnchorAWithRespectToBoxCentre));
	b2Vec2 AnchorB = Ermine::GLMToB2Vec2(Ermine::vertexErmineToWorld(AnchorBWithRespectToBoxCentre));

	b2RevoluteJointDef RevDef;

	RevDef.bodyA = BodyA;
	RevDef.bodyB = BodyB;
	RevDef.collideConnected = ShouldBodiesAttachedByTheJointCollide;

	RevDef.localAnchorA = AnchorA;
	RevDef.localAnchorB = AnchorB;

	RevDef.referenceAngle = glm::radians<float>(ReferenceAngleDegrees);

	JointHandle = (b2RevoluteJoint*)Ermine::Universum->CreateJoint(&RevDef);
}

Ermine::RevoluteJoint::RevoluteJoint(b2Joint* Pointer, std::string JointName, b2Body* BodyA, b2Body* BodyB)
	:
	JointBase(JointName, BodyA, BodyB)
{
	//The Pointer Is Constructed Externally.. That Makes Using This Constructor Dangerous.. Used For The Ruve Loader As Of This Reading..
	JointHandle = Pointer;
}

Ermine::RevoluteJoint::~RevoluteJoint()
{}

Ermine::RevoluteJoint::RevoluteJoint(RevoluteJoint && rhs)
	:
	JointBase(std::move(rhs))
{}

Ermine::RevoluteJoint& Ermine::RevoluteJoint::operator=(RevoluteJoint&& rhs)
{
	JointBase::operator=(std::move(rhs));
	return *this;
}

std::shared_ptr<Ermine::RevoluteJoint> Ermine::RevoluteJoint::Generate(std::string JointName, b2Body* BodyA, b2Body* BodyB, glm::vec2 AnchorAWithRespectToBoxCentre, glm::vec2 AnchorBWithRespectToBoxCentre, float ReferenceAngleDegrees, bool ShouldBodiesAttachedByTheJointCollide)
{
	std::shared_ptr<Ermine::RevoluteJoint> Joint(new Ermine::RevoluteJoint(JointName, BodyA, BodyB, AnchorAWithRespectToBoxCentre, AnchorBWithRespectToBoxCentre, ReferenceAngleDegrees, ShouldBodiesAttachedByTheJointCollide));
	return Joint;
}

std::shared_ptr<Ermine::RevoluteJoint> Ermine::RevoluteJoint::Generate(b2Joint* Pointer, std::string JointName, b2Body* BodyA, b2Body* BodyB)
{
	std::shared_ptr<Ermine::RevoluteJoint> Joint(new Ermine::RevoluteJoint(Pointer,JointName,BodyA,BodyB));
	return Joint;
}


bool Ermine::RevoluteJoint::LimitEnabled()
{
	if (JointBase::GetHealth() == Ermine::JointHealthEnum::StatusOk)
		return ((b2RevoluteJoint*)JointHandle)->IsLimitEnabled();
	else
	{
		STDOUTDefaultLog_Error("Cannot Check If Limits Are Enabled On Revolute Joint As Joint Health Is Not Okay..");
		return false;
	}
}

void Ermine::RevoluteJoint::EnableLimit()
{
	if (JointBase::GetHealth() == Ermine::JointHealthEnum::StatusOk)
		((b2RevoluteJoint*)JointHandle)->EnableLimit(true);
	else STDOUTDefaultLog_Error("Cannot EnableLimits On Revolute Joint As Joint Health Is Not Okay..");
}

void Ermine::RevoluteJoint::DisableLimit()
{
	if (JointBase::GetHealth() == Ermine::JointHealthEnum::StatusOk)
		((b2RevoluteJoint*)JointHandle)->EnableLimit(false);
	else STDOUTDefaultLog_Error("Cannot DisableLimits On Revolute Joint As Joint Health Is Not Okay..");
}

void Ermine::RevoluteJoint::ToggleJointLimits()
{
	if (JointBase::GetHealth() == Ermine::JointHealthEnum::StatusOk)
	{
		bool Limit = LimitEnabled();
		((b2RevoluteJoint*)JointHandle)->EnableLimit(!Limit);
	}
	else STDOUTDefaultLog_Error("Cannot Toggle JointLimits On Revolute Joint As Joint Health Is Not Okay..");
}

glm::vec2 Ermine::RevoluteJoint::GetRotationLimitsDegrees()
{
	if (JointBase::GetHealth() == Ermine::JointHealthEnum::StatusOk)
	{
		auto Limits = GetRotationLimitsRadians();
		float LowerLimit = glm::degrees<float>(Limits.x);
		float UpperLimit = glm::degrees<float>(Limits.y);
		return glm::vec2(LowerLimit, UpperLimit);
	}
	else
	{
		STDOUTDefaultLog_Error("Cannot Get Rotation JointLimits On Revolute Joint As Joint Health Is Not Okay..");
		return glm::vec2(-9999.0f, -9999.0f);
	}
	
}

glm::vec2 Ermine::RevoluteJoint::GetRotationLimitsRadians()
{
	if (JointBase::GetHealth() == Ermine::JointHealthEnum::StatusOk)
	{
		float UpperLimit = ((b2RevoluteJoint*)JointHandle)->GetUpperLimit();
		float LowerLimit = ((b2RevoluteJoint*)JointHandle)->GetLowerLimit();
		return glm::vec2(LowerLimit, UpperLimit);
	}
	else
	{
		STDOUTDefaultLog_Error("Cannot Get Rotation JointLimits On Revolute Joint As Joint Health Is Not Okay..");
		return glm::vec2(-9999.0f, -9999.0f);
	}
}

void Ermine::RevoluteJoint::SetRotationLimits(float LowerLimit, float UpperLimit,bool Degrees)
{
	if (JointBase::GetHealth() == Ermine::JointHealthEnum::StatusOk)
	{
		if (Degrees == true)
		{
			LowerLimit = glm::radians<float>(LowerLimit);
			UpperLimit = glm::radians<float>(UpperLimit);
		}
		((b2RevoluteJoint*)JointHandle)->SetLimits(LowerLimit, UpperLimit);
	}
	else STDOUTDefaultLog_Error("Cannot Set Rotation Limits On Revolute Joint As Joint Health Is Not Okay..");
}

float Ermine::RevoluteJoint::GetRotationLimitsUpperDegrees()
{
	if (JointBase::GetHealth() == Ermine::JointHealthEnum::StatusOk)
	{
		auto Limits = GetRotationLimitsDegrees();
		return Limits.y;
	}
	else
	{
		STDOUTDefaultLog_Error("Cannot Get Rotation Limits Upper On Revolute Joint As Joint Health Is Not Okay..");
		return -9999.0f;
	}
}

float Ermine::RevoluteJoint::GetRotationLimitsUpperRadians()
{
	if (JointBase::GetHealth() == Ermine::JointHealthEnum::StatusOk)
	{
		auto Limits = GetRotationLimitsRadians();
		return Limits.y;
	}
	else
	{
		STDOUTDefaultLog_Error("Cannot Get Rotation Limits Upper On Revolute Joint As Joint Health Is Not Okay..");
		return -9999.0f;
	}
}

void Ermine::RevoluteJoint::SetRotationLimitsUpper(float Angle, bool Degrees)
{
	if(Degrees == true)
		Angle = glm::radians<float>(Angle);
	
	if (JointBase::GetHealth() == Ermine::JointHealthEnum::StatusOk)
	{
		float LowerLimit = ((b2RevoluteJoint*)JointHandle)->GetLowerLimit();
		if (LowerLimit < -99999.0f)
			LowerLimit = -99999.0f;

		((b2RevoluteJoint*)JointHandle)->SetLimits(LowerLimit, Angle);
	}
	else STDOUTDefaultLog_Error("Cannot Set Rotation Limit Upper On Revolute Joint As Joint Health Is Not Okay..");

	((b2RevoluteJoint*)JointHandle)->GetLowerLimit();
}
float Ermine::RevoluteJoint::GetRotationLimitsLowerDegrees()
{
	if (JointBase::GetHealth() == Ermine::JointHealthEnum::StatusOk)
	{
		auto Limits = GetRotationLimitsDegrees();
		return Limits.x;
	}
	else
	{
		STDOUTDefaultLog_Error("Cannot Get Rotation Limits Lower On Revolute Joint As Joint Health Is Not Okay..");
		return -9999.0f;
	}
}
float Ermine::RevoluteJoint::GetRotationLimitsLowerRadians()
{
	if (JointBase::GetHealth() == Ermine::JointHealthEnum::StatusOk)
	{
		auto Limits = GetRotationLimitsRadians();
		return Limits.x;
	}
	else
	{
		STDOUTDefaultLog_Error("Cannot Get Rotation Limits Lower On Revolute Joint As Joint Health Is Not Okay..");
		return -9999.0f;
	}
}
void Ermine::RevoluteJoint::SetRotationLimitsLower(float Angle, bool Degrees)
{
	if (Degrees == true)
		Angle = glm::radians<float>(Angle);

	if (JointBase::GetHealth() == Ermine::JointHealthEnum::StatusOk)
	{
		float UpperLimit = ((b2RevoluteJoint*)JointHandle)->GetLowerLimit();
		if (UpperLimit > 99999.0f)
			UpperLimit = 99999.0f;

		((b2RevoluteJoint*)JointHandle)->SetLimits(Angle, UpperLimit);
	}
	else STDOUTDefaultLog_Error("Cannot Set Rotation Limit Lower On Revolute Joint As Joint Health Is Not Okay..");
}


glm::vec2 Ermine::RevoluteJoint::GetBodyALocalAnchorLocation()
{
	if (JointBase::GetHealth() == Ermine::JointHealthEnum::StatusOk)
	{
		b2Vec2 LocalAnchorLocation = ((b2RevoluteJoint*)JointHandle)->GetLocalAnchorA();
		glm::vec2 LocalAnchorLocPixel = Ermine::vertexWorldToErmine(B2Vec2ToGLM(LocalAnchorLocation));
		return LocalAnchorLocPixel;
	}
	else
	{
		STDOUTDefaultLog_Error("Cannot Get BodyALocalAnchorLocation On Revolute Joint As Joint Health Is Not Okay..");
		return glm::vec2(-9999.0f, -9999.0f);
	}
}

glm::vec2 Ermine::RevoluteJoint::GetBodyBLocalAnchorLocation()
{
	if (JointBase::GetHealth() == Ermine::JointHealthEnum::StatusOk)
	{
		b2Vec2 LocalAnchorLocation = ((b2RevoluteJoint*)JointHandle)->GetLocalAnchorB();
		glm::vec2 LocalAnchorLocPixel = Ermine::vertexWorldToErmine(B2Vec2ToGLM(LocalAnchorLocation));
		return LocalAnchorLocPixel;
	}
	else
	{
		STDOUTDefaultLog_Error("Cannot Get BodyBLocalAnchorLocation On Revolute Joint As Joint Health Is Not Okay..");
		return glm::vec2(-9999.0f, -9999.0f);
	}
}

#pragma region MotorFunctions

void Ermine::RevoluteJoint::EnableMotor(bool enabled)
{
	if(JointBase::GetHealth() == Ermine::JointHealthEnum::StatusOk)
		((b2RevoluteJoint*)JointHandle)->EnableMotor(enabled);
	else STDOUTDefaultLog_Error("Cannot Enable Motor On Revolute Joint As Joint Health Is Not Okay..");
}
void Ermine::RevoluteJoint::SetMotorSpeed(float speed)
{
	if(JointBase::GetHealth() == Ermine::JointHealthEnum::StatusOk)
		((b2RevoluteJoint*)JointHandle)->SetMotorSpeed(speed);
	else STDOUTDefaultLog_Error("Cannot Set Motor Speed On Revolute Joint As Joint Health Is Not Okay..");
}
void Ermine::RevoluteJoint::SetMaxMotorTorque(float torque)
{
	if(JointBase::GetHealth() == Ermine::JointHealthEnum::StatusOk)
		((b2RevoluteJoint*)JointHandle)->SetMaxMotorTorque(torque);
	else STDOUTDefaultLog_Error("Cannot Set Max Motor Torque On Revolute Joint As Joint Health Is Not Okay..");
}

bool Ermine::RevoluteJoint::IsMotorEnabled()
{
	if (JointBase::GetHealth() == Ermine::JointHealthEnum::StatusOk)
		return ((b2RevoluteJoint*)JointHandle)->IsMotorEnabled();
	else
	{
		STDOUTDefaultLog_Error("Cannot Query Motor Enabled On Revolute Joint As Joint Health Is Not Okay..");
		return false;
	}
}
float Ermine::RevoluteJoint::GetMotorSpeed()
{
	if (JointBase::GetHealth() == Ermine::JointHealthEnum::StatusOk)
		return ((b2RevoluteJoint*)JointHandle)->GetMotorSpeed();
	else
	{
		STDOUTDefaultLog_Error("Cannot Query Motor Speed On Revolute Joint As Joint Health Is Not Okay..");
		return -9999.0f;
	}
}
float Ermine::RevoluteJoint::GetMotorTorque()
{
	if (JointBase::GetHealth() == Ermine::JointHealthEnum::StatusOk)
		return ((b2RevoluteJoint*)JointHandle)->GetMotorTorque(1.0f / 0.04f);
	else
	{
		STDOUTDefaultLog_Error("Cannot Query Motor Torque On Revolute Joint As Joint Health Is Not Okay..");
		return -9999.0f;
	}
}

#pragma endregion MotorFunctions


float Ermine::RevoluteJoint::GetJointSpeedDegreesPerSecond()
{
	if (JointBase::GetHealth() == Ermine::JointHealthEnum::StatusOk)
		return glm::degrees<float>(GetJointSpeedRadiansPerSecond());
	else
	{
		STDOUTDefaultLog_Error("Cannot Query JointSpeed On Revolute Joint As Joint Health Is Not Okay..");
		return 9999.0f;
	}
}

float Ermine::RevoluteJoint::GetJointSpeedRadiansPerSecond()
{
	if (JointBase::GetHealth() == Ermine::JointHealthEnum::StatusOk)
		return ((b2RevoluteJoint*)JointHandle)->GetJointSpeed();
	else 
	{
		STDOUTDefaultLog_Error("Cannot Query JointSpeed On Revolute Joint As Joint Health Is Not Okay..");
		return 9999.0f;
	}
	
}

float Ermine::RevoluteJoint::GetReferenceAngleInDegrees()
{
	if (JointBase::GetHealth() == Ermine::JointHealthEnum::StatusOk)
		return glm::degrees<float>(GetReferenceAngleInRadians());
	else 
	{
		STDOUTDefaultLog_Error("Cannot Query ReferenceAngle On Revolute Joint As Joint Health Is Not Okay..");
		return -9999.0f;
	}
}
float Ermine::RevoluteJoint::GetReferenceAngleInRadians()
{
	if (JointBase::GetHealth() == Ermine::JointHealthEnum::StatusOk)
		return ((b2RevoluteJoint*)JointHandle)->GetReferenceAngle();
	else
	{
		STDOUTDefaultLog_Error("Cannot Query ReferenceAngle On Revolute Joint As Joint Health Is Not Okay..");
		return -9999.0f;
	}
}

float Ermine::RevoluteJoint::GetJointAngleInDegrees()
{
	if (JointBase::GetHealth() == Ermine::JointHealthEnum::StatusOk)
		return glm::degrees<float>(GetJointAngleInRadians());
	else
	{
		STDOUTDefaultLog_Error("Cannot Query JointAngle On Revolute Joint As Joint Health Is Not Okay..");
		return 9999.0f;
	}
}

float Ermine::RevoluteJoint::GetJointAngleInRadians()
{
	if (JointBase::GetHealth() == Ermine::JointHealthEnum::StatusOk)
		return ((b2RevoluteJoint*)JointHandle)->GetJointAngle();
	else
	{
		STDOUTDefaultLog_Error("Cannot Query JointAngle On Revolute Joint As Joint Health Is Not Okay..");
		return 9999.0f;
	}
}