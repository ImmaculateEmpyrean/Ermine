#include "stdafx.h"
#include "PrismaticJoint.h"

Ermine::PrismaticJoint::PrismaticJoint(b2Body* BodyA, b2Body* BodyB, bool ShouldBodiesAttachedByTheJointCollide)
	:
	JointBase(BodyA,BodyB)
{
	HelperConstructPrismaticJoint(BodyA, BodyB, glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f),
								  glm::vec2(1.0f, 0.0f), 0.0f, ShouldBodiesAttachedByTheJointCollide);
}

Ermine::PrismaticJoint::PrismaticJoint(b2Body* BodyA, b2Body* BodyB, glm::vec2 AnchorAWithRespectToBoxCentre, bool ShouldBodiesAttachedByTheJointCollide)
	:
	JointBase(BodyA,BodyB)
{
	HelperConstructPrismaticJoint(BodyA, BodyB, AnchorAWithRespectToBoxCentre, glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f), 0.0f, ShouldBodiesAttachedByTheJointCollide);
}

Ermine::PrismaticJoint::PrismaticJoint(b2Body* BodyA, b2Body* BodyB, glm::vec2 AnchorAWithRespectToBoxCentre, glm::vec2 AnchorBWithRespectToBoxCentre, bool ShouldBodiesAttachedByTheJointCollide)
	:
	JointBase(BodyA,BodyB)
{
	HelperConstructPrismaticJoint(BodyA, BodyB, AnchorAWithRespectToBoxCentre,AnchorBWithRespectToBoxCentre,
								 glm::vec2(1.0f, 0.0f), 0.0f, ShouldBodiesAttachedByTheJointCollide);
}

Ermine::PrismaticJoint::PrismaticJoint(b2Body* BodyA, b2Body* BodyB, glm::vec2 AnchorAWithRespectToBoxCentre, glm::vec2 AnchorBWithRespectToBoxCentre, float ReferenceAngleDegrees, bool ShouldBodiesAttachedByTheJointCollide)
	:
	JointBase(BodyA,BodyB)
{
	HelperConstructPrismaticJoint(BodyA, BodyB, AnchorAWithRespectToBoxCentre, AnchorBWithRespectToBoxCentre,
		glm::vec2(1.0f, 0.0f), ReferenceAngleDegrees, ShouldBodiesAttachedByTheJointCollide);
}

Ermine::PrismaticJoint::PrismaticJoint(b2Body* BodyA, b2Body* BodyB, glm::vec2 AnchorAWithRespectToBoxCentre, glm::vec2 AnchorBWithRespectToBoxCentre, float ReferenceAngleDegrees, glm::vec2 SlidingAxis, bool ShouldBodiesAttachedByTheJointCollide)
	:
	JointBase(BodyA,BodyB)
{
	HelperConstructPrismaticJoint(BodyA, BodyB, AnchorAWithRespectToBoxCentre, AnchorBWithRespectToBoxCentre,
		SlidingAxis, ReferenceAngleDegrees, ShouldBodiesAttachedByTheJointCollide);
}

Ermine::PrismaticJoint::~PrismaticJoint()
{
	if (ValidFlag == true)
	{
		Ermine::Universum->DestroyJoint(PrismaticJointHandle);
		PrismaticJointHandle = nullptr;
	}
}


void Ermine::PrismaticJoint::SetMovementLimits(float LowerLimitMovement, float UpperLimitMovement)
{
	float WorldMovementConstraintLower = Ermine::scalarPixelsToWorld(LowerLimitMovement);
	float WorldMovementConstraintUpper = Ermine::scalarPixelsToWorld(UpperLimitMovement);

	PrismaticJointHandle->EnableLimit(true);
	PrismaticJointHandle->SetLimits(WorldMovementConstraintLower,WorldMovementConstraintUpper);
}
void Ermine::PrismaticJoint::SetMovementLimits(float LimitMovement, bool IsUpperLimit)
{
	float WorldMovementConstraint = Ermine::scalarPixelsToWorld(LimitMovement);

	PrismaticJointHandle->EnableLimit(true);

	if (IsUpperLimit == true)
		PrismaticJointHandle->SetLimits(-99999.0f, WorldMovementConstraint);
	else
		PrismaticJointHandle->SetLimits(WorldMovementConstraint, 99999.0f);
}

void Ermine::PrismaticJoint::ClearLimits()
{
	PrismaticJointHandle->EnableLimit(false);
}


glm::vec2 Ermine::PrismaticJoint::GetBodyALocalAnchorLocation()
{
	if (ValidFlag == true)
	{
		b2Vec2 LocalAnchorLocation = PrismaticJointHandle->GetLocalAnchorA();
		glm::vec2 LocalAnchorLocPixel = Ermine::vertexWorldToPixels(B2Vec2ToGLM(LocalAnchorLocation));
		return LocalAnchorLocPixel;
	}
	else return glm::vec2(-9999.0f, -9999.0f);
}
glm::vec2 Ermine::PrismaticJoint::GetBodyBLocalAnchorLocation()
{
	if (ValidFlag == true)
	{
		b2Vec2 LocalAnchorLocation = PrismaticJointHandle->GetLocalAnchorB();
		glm::vec2 LocalAnchorLocPixel = Ermine::vertexWorldToPixels(B2Vec2ToGLM(LocalAnchorLocation));
		return LocalAnchorLocPixel;
	}
	else return glm::vec2(-9999.0f, -9999.0f);
}

glm::vec2 Ermine::PrismaticJoint::GetBodyAWorldAnchorLocationPixels()
{
	if (ValidFlag == true)
	{
		b2Vec2 LocalAnchorLocation = PrismaticJointHandle->GetAnchorA();
		glm::vec2 LocalAnchorLocPixel = Ermine::coordWorldToPixels(B2Vec2ToGLM(LocalAnchorLocation));
		return LocalAnchorLocPixel;
	}
	else return glm::vec2(-9999.0f, -9999.0f);
}
glm::vec2 Ermine::PrismaticJoint::GetBodyBWorldAnchorLocationPixels()
{
	if (ValidFlag == true)
	{
		b2Vec2 LocalAnchorLocation = PrismaticJointHandle->GetAnchorB();
		glm::vec2 LocalAnchorLocPixel = Ermine::coordWorldToPixels(B2Vec2ToGLM(LocalAnchorLocation));
		return LocalAnchorLocPixel;
	}
	else return glm::vec2(-9999.0f, -9999.0f);
}


#pragma region MotorFunctions
void Ermine::PrismaticJoint::EnableMotor(bool enabled)
{
	if(ValidFlag == true)
		PrismaticJointHandle->EnableMotor(true);
}
void Ermine::PrismaticJoint::SetMotorSpeed(float speed)
{
	if (ValidFlag == true)
	{
		PrismaticJointHandle->EnableMotor(true);
		PrismaticJointHandle->SetMotorSpeed(speed);
	}
}
void Ermine::PrismaticJoint::SetMaxMotorForce(float Force)
{
	if (ValidFlag == true)
	{
		PrismaticJointHandle->SetMaxMotorForce(Force);
	}
}

bool Ermine::PrismaticJoint::IsMotorEnabled()
{
	if (ValidFlag == true)
		return PrismaticJointHandle->IsMotorEnabled();
	else return false;
}
float Ermine::PrismaticJoint::GetMotorSpeed()
{
	if (ValidFlag == true)
		return PrismaticJointHandle->GetMotorSpeed();
	else return 9999.0f;
}
float Ermine::PrismaticJoint::GetMotorForce()
{
	if (ValidFlag == true)
		return PrismaticJointHandle->GetMotorForce(1.0f / Ermine::PhysicsWorldTimestep);
	else return 9999.0f;
}

#pragma endregion MotorFunctions

float Ermine::PrismaticJoint::GetReferenceAngleInDegrees()
{
	if (ValidFlag == true)
		return glm::degrees<float>(GetReferenceAngleInRadians());
	else return 9999.0f;
}

float Ermine::PrismaticJoint::GetReferenceAngleInRadians()
{
	if (ValidFlag == true)
		return PrismaticJointHandle->GetReferenceAngle();
	else return 9999.0f;
}

void Ermine::PrismaticJoint::HelperConstructPrismaticJoint(b2Body* BodyA, b2Body* BodyB, glm::vec2 AnchorAWithRespectToBoxCentre, glm::vec2 AnchorBWithRespectToBoxCentre, 
																						 glm::vec2 SlidingAxis,float ReferenceAngleDegrees ,bool ShouldBodiesAttachedByTheJointCollide)
{
	glm::vec2 LocalAnchorAWorldSpace = Ermine::vertexPixelsToWorld(AnchorAWithRespectToBoxCentre);
	glm::vec2 LocalAnchorBWorldSpace = Ermine::vertexPixelsToWorld(AnchorBWithRespectToBoxCentre);

	glm::vec2 LocalAxisAWorldSpace = Ermine::vertexPixelsToWorld(SlidingAxis);
	glm::vec2 NormalizedLocalAxisAWorldSpace = glm::normalize(LocalAxisAWorldSpace);
	
	float ReferenceAngleRadians = glm::radians<float>(ReferenceAngleDegrees);

	b2PrismaticJointDef PrDef;

	PrDef.bodyA = BodyA;
	PrDef.bodyB = BodyB;
	PrDef.collideConnected = ShouldBodiesAttachedByTheJointCollide;

	PrDef.localAnchorA = b2Vec2(LocalAnchorAWorldSpace.x, LocalAnchorAWorldSpace.y);
	PrDef.localAnchorB = b2Vec2(LocalAnchorBWorldSpace.x, LocalAnchorBWorldSpace.y);

	PrDef.localAxisA = b2Vec2(NormalizedLocalAxisAWorldSpace.x, NormalizedLocalAxisAWorldSpace.y);

	PrDef.referenceAngle = ReferenceAngleRadians;

	PrismaticJointHandle = (b2PrismaticJoint*)Universum->CreateJoint(&PrDef);

	JointBase::JointHandle = PrismaticJointHandle;
}
