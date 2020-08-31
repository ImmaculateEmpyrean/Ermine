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
	if (PrismaticJointHandle != nullptr)
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


glm::vec2 Ermine::PrismaticJoint::GetAnchorALocLocalSpace()
{
	glm::vec2 AnchorALocalPosition = glm::vec2(PrismaticJointHandle->GetLocalAnchorA().x, PrismaticJointHandle->GetLocalAnchorA().y);
	return Ermine::vertexWorldToPixels(AnchorALocalPosition);
}
glm::vec2 Ermine::PrismaticJoint::GetAnchorBLocLocalSpace()
{
	glm::vec2 AnchorBLocalPosition = glm::vec2(PrismaticJointHandle->GetLocalAnchorB().x, PrismaticJointHandle->GetLocalAnchorB().y);
	return Ermine::vertexWorldToPixels(AnchorBLocalPosition);
}

glm::vec2 Ermine::PrismaticJoint::GetAnchorALocPixelSpace()
{
	glm::vec2 AnchorAWorldPosition = glm::vec2(PrismaticJointHandle->GetAnchorA().x, PrismaticJointHandle->GetAnchorA().y);
	return Ermine::coordWorldToPixels(AnchorAWorldPosition);
}
glm::vec2 Ermine::PrismaticJoint::GetAnchorBLocPixelSpace()
{
	glm::vec2 AnchorBWorldPosition = glm::vec2(PrismaticJointHandle->GetAnchorB().x, PrismaticJointHandle->GetAnchorB().y);
	return Ermine::coordWorldToPixels(AnchorBWorldPosition);
}


#pragma region MotorFunctions
void Ermine::PrismaticJoint::EnableMotor(bool enabled)
{
	PrismaticJointHandle->EnableMotor(true);
}
void Ermine::PrismaticJoint::SetMotorSpeed(float speed)
{
	PrismaticJointHandle->EnableMotor(true);
	PrismaticJointHandle->SetMotorSpeed(speed);
}
void Ermine::PrismaticJoint::SetMaxMotorForce(float Force)
{
	PrismaticJointHandle->SetMaxMotorForce(Force);
}

bool Ermine::PrismaticJoint::IsMotorEnabled()
{
	return PrismaticJointHandle->IsMotorEnabled();
}
float Ermine::PrismaticJoint::GetMotorSpeed()
{
	return PrismaticJointHandle->GetMotorSpeed();
}
float Ermine::PrismaticJoint::GetMotorForce()
{
	return PrismaticJointHandle->GetMotorForce(1.0f/Ermine::PhysicsWorldTimestep);
}

#pragma endregion MotorFunctions

float Ermine::PrismaticJoint::GetReferenceAngleInDegrees()
{
	return glm::degrees<float>(GetReferenceAngleInRadians());
}

float Ermine::PrismaticJoint::GetReferenceAngleInRadians()
{
	return PrismaticJointHandle->GetReferenceAngle();
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
}
