#include "stdafx.h"
#include "PrismaticJoint.h"

Ermine::PrismaticJoint::PrismaticJoint(std::string JointName,b2Body* BodyA, b2Body* BodyB, glm::vec2 AnchorAWithRespectToBoxCentre, glm::vec2 AnchorBWithRespectToBoxCentre, float ReferenceAngleDegrees, glm::vec2 SlidingAxis, bool ShouldBodiesAttachedByTheJointCollide)
	:
	JointBase(JointName,BodyA,BodyB)
{
	b2Vec2 AnchorA = Ermine::GLMToB2Vec2(Ermine::vertexErmineToWorld(AnchorAWithRespectToBoxCentre));
	b2Vec2 AnchorB = Ermine::GLMToB2Vec2(Ermine::vertexErmineToWorld(AnchorBWithRespectToBoxCentre));

	glm::vec2 LocalAxisAWorldSpace = Ermine::vertexErmineToWorld(SlidingAxis);
	glm::vec2 NormalizedLocalAxisAWorldSpace = glm::normalize(LocalAxisAWorldSpace);

	b2PrismaticJointDef PrDef;

	PrDef.bodyA = BodyA;
	PrDef.bodyB = BodyB;
	PrDef.collideConnected = ShouldBodiesAttachedByTheJointCollide;

	PrDef.localAnchorA = AnchorA;
	PrDef.localAnchorB = AnchorB;

	PrDef.localAxisA = b2Vec2(NormalizedLocalAxisAWorldSpace.x, NormalizedLocalAxisAWorldSpace.y);

	PrDef.referenceAngle = glm::radians<float>(ReferenceAngleDegrees);

	JointHandle = (b2PrismaticJoint*)Universum->CreateJoint(&PrDef);
}

Ermine::PrismaticJoint::~PrismaticJoint()
{}

Ermine::PrismaticJoint::PrismaticJoint(PrismaticJoint && rhs)
	:
	JointBase(std::move(rhs))
{}

Ermine::PrismaticJoint& Ermine::PrismaticJoint::operator=(PrismaticJoint&& rhs)
{
	JointBase::operator=(std::move(rhs));
	return *this;
}

std::shared_ptr<Ermine::PrismaticJoint> Ermine::PrismaticJoint::Generate(std::string JointName, b2Body* BodyA, b2Body* BodyB, glm::vec2 AnchorAWithRespectToBoxCentre, glm::vec2 AnchorBWithRespectToBoxCentre, float ReferenceAngleDegrees, glm::vec2 SlidingAxis, bool ShouldBodiesAttachedByTheJointCollide)
{
	std::shared_ptr<Ermine::PrismaticJoint> Ptr(new Ermine::PrismaticJoint(JointName, BodyA, BodyB, AnchorAWithRespectToBoxCentre, AnchorBWithRespectToBoxCentre, ReferenceAngleDegrees, SlidingAxis, ShouldBodiesAttachedByTheJointCollide),Ermine::JointDeleter<Ermine::PrismaticJoint>());
	return Ptr;
}

void Ermine::PrismaticJoint::EnableLimits()
{
	if (GetHealth() == Ermine::JointHealthEnum::StatusOk)
		((b2PrismaticJoint*)JointHandle)->EnableLimit(true);
	else STDOUTDefaultLog_Error("Cannot Enable Movement Limit Of The Prismatic Joint As Object Health Is Not OK");
}

void Ermine::PrismaticJoint::DisableLimits()
{
	if (GetHealth() == Ermine::JointHealthEnum::StatusOk)
		((b2PrismaticJoint*)JointHandle)->EnableLimit(false);
	else STDOUTDefaultLog_Error("Cannot Enable Movement Limit Of The Prismatic Joint As Object Health Is Not OK");
}

void Ermine::PrismaticJoint::ToggleLimitFlag()
{
	if (GetHealth() == Ermine::JointHealthEnum::StatusOk)
	{
		auto Flag = ((b2PrismaticJoint*)JointHandle)->IsEnabled();
		((b2PrismaticJoint*)JointHandle)->EnableLimit(!Flag);
	}
	else STDOUTDefaultLog_Error("Cannot Enable Movement Limit Of The Prismatic Joint As Object Health Is Not OK");
}

glm::vec2 Ermine::PrismaticJoint::GetMovementLimits()
{
	if (GetHealth() == Ermine::JointHealthEnum::StatusOk)
	{
		glm::vec2 Limits = glm::vec2(0.0f);
		Limits.x = Ermine::scalarWorldToErmine(((b2PrismaticJoint*)JointHandle)->GetLowerLimit());
		Limits.y = Ermine::scalarWorldToErmine(((b2PrismaticJoint*)JointHandle)->GetLowerLimit());
		return Limits;
	}
	else
	{
		STDOUTDefaultLog_Error("Cannot Query Movement Limit Of The Prismatic Joint As Object Health Is Not OK");
		return glm::vec2(-9999.0f, -9999.0f);
	}
}

float Ermine::PrismaticJoint::GetLowerLimitMovement()
{
	if (GetHealth() == Ermine::JointHealthEnum::StatusOk)
	{
		auto Limits = GetMovementLimits();
		return Limits.x;
	}
	else
	{
		STDOUTDefaultLog_Error("Cannot Query Lower Movement Limit Of The Prismatic Joint As Object Health Is Not OK");
		return -9999.0f;
	}
}

float Ermine::PrismaticJoint::GetUpperLimitMovement()
{
	if (GetHealth() == Ermine::JointHealthEnum::StatusOk)
	{
		auto Limits = GetMovementLimits();
		return Limits.y;
	}
	else
	{
		STDOUTDefaultLog_Error("Cannot Query Upper Movement Limit Of The Prismatic Joint As Object Health Is Not OK");
		return -9999.0f;
	}
}

void Ermine::PrismaticJoint::SetMovementLimits(glm::vec2 Limits)
{
	SetMovementLimits(Limits.x,Limits.y);
}

void Ermine::PrismaticJoint::SetMovementLimits(float LowerLimitMovement, float UpperLimitMovement)
{
	if (GetHealth() == Ermine::JointHealthEnum::StatusOk)
	{
		float WorldMovementConstraintLower = Ermine::scalarErmineToWorld(LowerLimitMovement);
		float WorldMovementConstraintUpper = Ermine::scalarErmineToWorld(UpperLimitMovement);
		((b2PrismaticJoint*)JointHandle)->SetLimits(WorldMovementConstraintLower, WorldMovementConstraintUpper);
	}
	else
	{
		STDOUTDefaultLog_Error("Cannot Set Movement Limits Of The Prismatic Joint As Object Health Is Not OK");
	}
}
void Ermine::PrismaticJoint::SetMovementLimitsUpper(float UpperLimit)
{
	if (GetHealth() == Ermine::JointHealthEnum::StatusOk)
	{
		float WorldMovementConstraint = Ermine::scalarErmineToWorld(UpperLimit);

		float LowerLimit = ((b2PrismaticJoint*)JointHandle)->GetLowerLimit();
		if (LowerLimit < -9999.0f)
			LowerLimit = -9999.0f;

		((b2PrismaticJoint*)JointHandle)->SetLimits(LowerLimit, WorldMovementConstraint);
	}
	else
	{
		STDOUTDefaultLog_Error("Cannot Set Lower Movement Limit Of The Prismatic Joint As Object Health Is Not OK");
	}
}
void Ermine::PrismaticJoint::SetMovementLimitsLower(float LowerLimit)
{
	if (GetHealth() == Ermine::JointHealthEnum::StatusOk)
	{
		float WorldMovementConstraint = Ermine::scalarErmineToWorld(LowerLimit);

		float UpperLimit = ((b2PrismaticJoint*)JointHandle)->GetUpperLimit();
		if (UpperLimit > 9999.0f)
			UpperLimit = 9999.0f;

		((b2PrismaticJoint*)JointHandle)->SetLimits(WorldMovementConstraint, UpperLimit);
	}
	else
	{
		STDOUTDefaultLog_Error("Cannot Set Higher Movement Limit Of The Prismatic Joint As Object Health Is Not OK");
	}
}

glm::vec2 Ermine::PrismaticJoint::GetBodyALocalAnchorLocation()
{
	if (GetHealth() == Ermine::JointHealthEnum::StatusOk)
	{
		b2Vec2 LocalAnchorLocation = ((b2PrismaticJoint*)JointHandle)->GetLocalAnchorA();
		glm::vec2 LocalAnchorLocPixel = Ermine::vertexWorldToErmine(B2Vec2ToGLM(LocalAnchorLocation));
		return LocalAnchorLocPixel;
	}
	else
	{
		STDOUTDefaultLog_Error("Cannot Get BodyA LocalAnchor Location Of The Prismatic Joint IN Question As The Object Health Is Not Okay");
		return glm::vec2(-9999.0f, -9999.0f);
	}
}
glm::vec2 Ermine::PrismaticJoint::GetBodyBLocalAnchorLocation()
{
	if (GetHealth() == Ermine::JointHealthEnum::StatusOk)
	{
		b2Vec2 LocalAnchorLocation = ((b2PrismaticJoint*)JointHandle)->GetLocalAnchorB();
		glm::vec2 LocalAnchorLocPixel = Ermine::vertexWorldToErmine(B2Vec2ToGLM(LocalAnchorLocation));
		return LocalAnchorLocPixel;
	}
	else
	{
		STDOUTDefaultLog_Error("Cannot Get BodyB LocalAnchor Location Of The Prismatic Joint IN Question As The Object Health Is Not Okay");
		return glm::vec2(-9999.0f, -9999.0f);
	}
}

#pragma region MotorFunctions
void Ermine::PrismaticJoint::EnableMotor(bool enabled)
{
	if (GetHealth() == Ermine::JointHealthEnum::StatusOk)
		((b2PrismaticJoint*)JointHandle)->EnableMotor(true);
	else STDOUTDefaultLog_Error("Could Not Enable Motor Of The Prismatic Joint As Joint Health Is Not Okay..");
}
void Ermine::PrismaticJoint::SetMotorSpeed(float speed)
{
	if (GetHealth() == Ermine::JointHealthEnum::StatusOk)
	{
		((b2PrismaticJoint*)JointHandle)->EnableMotor(true);
		((b2PrismaticJoint*)JointHandle)->SetMotorSpeed(Ermine::vertexErmineToWorld(speed,0.0f).x);
	}
	else STDOUTDefaultLog_Error("Could Not Set Motor Speed Of The Prismatic Joint As Joint Health Is Not Okay..");
}
void Ermine::PrismaticJoint::SetMaxMotorForce(float Force)
{
	if (GetHealth() == Ermine::JointHealthEnum::StatusOk)
	{
		((b2PrismaticJoint*)JointHandle)->SetMaxMotorForce(Force);
	}
	else STDOUTDefaultLog_Error("Could Not Set Max MotorSpeed Of The Prismatic Joint As Joint Health Is Not Okay..");
}

bool Ermine::PrismaticJoint::IsMotorEnabled()
{
	if (GetHealth() == Ermine::JointHealthEnum::StatusOk)
		return ((b2PrismaticJoint*)JointHandle)->IsMotorEnabled();
	else
	{
		STDOUTDefaultLog_Error("Could Not Query Enable Motor Of The Prismatic Joint As Joint Health Is Not Okay..");
		return false;
	}
}
float Ermine::PrismaticJoint::GetMotorSpeed()
{
	if (GetHealth() == Ermine::JointHealthEnum::StatusOk)
	{
		if (((b2PrismaticJoint*)JointHandle)->IsMotorEnabled() != true)
			STDOUTDefaultLog_Warn("Querying Speed Of A Motor Not Enabled In Prismatic Joint");
		return ((b2PrismaticJoint*)JointHandle)->GetMotorSpeed();
	}
	else 
	{
		STDOUTDefaultLog_Error("Could Not Query Motor Speed Of The Prismatic Joint As Joint Health Is Not Okay..");
		return 9999.0f;
	}
}

float Ermine::PrismaticJoint::GetMotorForce()
{
	if (GetHealth() == Ermine::JointHealthEnum::StatusOk)
	{
		if (((b2PrismaticJoint*)JointHandle)->IsMotorEnabled() != true)
		{
			STDOUTDefaultLog_Warn("Querying Force Of A Motor Not Enabled In Prismatic Joint");
		}
		return ((b2PrismaticJoint*)JointHandle)->GetMotorForce(1.0f / 0.04f); //Physics World Timestep Is Locked To 0.04.. 25fps
	}
	else 
	{
		STDOUTDefaultLog_Error("Could Not Query Motor Force Of The Prismatic Joint As Joint Health Is Not Okay..");
		return 9999.0f;
	}
}
#pragma endregion MotorFunctions

float Ermine::PrismaticJoint::GetReferenceAngleInDegrees()
{
	if (GetHealth() == Ermine::JointHealthEnum::StatusOk)
		return glm::degrees<float>(GetReferenceAngleInRadians());
	else
	{
		STDOUTDefaultLog_Error("Could Not Query Reference Angle Of The Prismatic Joint As Joint Health Is Not Okay..");
		return 9999.0f;
	}
}
float Ermine::PrismaticJoint::GetReferenceAngleInRadians()
{
	if (GetHealth() == Ermine::JointHealthEnum::StatusOk)
		return ((b2PrismaticJoint*)JointHandle)->GetReferenceAngle();
	else
	{
		STDOUTDefaultLog_Error("Could Not Query Reference Angle Of The Prismatic Joint As Joint Health Is Not Okay..");
		 return 9999.0f;
	}
}