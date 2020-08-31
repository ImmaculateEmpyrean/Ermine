#include "stdafx.h"
#include "RevoluteJoint.h"

Ermine::RevoluteJoint::RevoluteJoint(b2Body* BodyA, b2Body* BodyB, bool ShouldBodiesAttachedByTheJointCollide)
	:
	JointBase(BodyA,BodyB)
{
	HelperConstructRevoluteJoint(BodyA, BodyB, glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), 0.0f, ShouldBodiesAttachedByTheJointCollide);
}

Ermine::RevoluteJoint::RevoluteJoint(b2Body* BodyA, b2Body* BodyB, glm::vec2 AnchorAWithRespectToBoxCentre, bool ShouldBodiesAttachedByTheJointCollide)
	:
	JointBase(BodyA, BodyB)
{
	HelperConstructRevoluteJoint(BodyA, BodyB, AnchorAWithRespectToBoxCentre, glm::vec2(0.0f, 0.0f), 0.0f, ShouldBodiesAttachedByTheJointCollide);
}

Ermine::RevoluteJoint::RevoluteJoint(b2Body* BodyA, b2Body* BodyB, glm::vec2 AnchorAWithRespectToBoxCentre, glm::vec2 AnchorBWithRespectToBoxCentre, bool ShouldBodiesAttachedByTheJointCollide)
	:
	JointBase(BodyA, BodyB)
{
	HelperConstructRevoluteJoint(BodyA, BodyB, AnchorAWithRespectToBoxCentre, AnchorAWithRespectToBoxCentre, 0.0f, ShouldBodiesAttachedByTheJointCollide);
}

Ermine::RevoluteJoint::RevoluteJoint(b2Body* BodyA, b2Body* BodyB, glm::vec2 AnchorAWithRespectToBoxCentre, glm::vec2 AnchorBWithRespectToBoxCentre, float ReferenceAngleDegrees, bool ShouldBodiesAttachedByTheJointCollide)
	:
	JointBase(BodyA, BodyB)
{
	float ReferenceAngle = glm::radians<float>(ReferenceAngleDegrees);
	HelperConstructRevoluteJoint(BodyA, BodyB, AnchorAWithRespectToBoxCentre, AnchorAWithRespectToBoxCentre, ReferenceAngle, ShouldBodiesAttachedByTheJointCollide);
}

Ermine::RevoluteJoint::~RevoluteJoint()
{
	if (RevoluteJointHandle != nullptr)
	{
		Ermine::Universum->DestroyJoint(RevoluteJointHandle);
		RevoluteJointHandle = nullptr;
	}
}


void Ermine::RevoluteJoint::SetRotationLimits(float LowerLimitDegrees, float UpperLimitDegrees)
{
	float LowerLimitRadians = glm::radians<float>(LowerLimitDegrees);
	float UpperLimitRadians = glm::radians<float>(UpperLimitDegrees);

	RevoluteJointHandle->EnableLimit(true);
	RevoluteJointHandle->SetLimits(LowerLimitRadians, UpperLimitRadians);
}
void Ermine::RevoluteJoint::SetRotationLimits(float LimitDegrees, bool IsUpperLimit)
{
	float LimitRadians = glm::radians<float>(LimitDegrees);
	
	RevoluteJointHandle->EnableLimit(true);

	if(IsUpperLimit == true)
		RevoluteJointHandle->SetLimits(-99999.0f, LimitRadians);
	else 
		RevoluteJointHandle->SetLimits(LimitRadians, 99999.0f);
}

void Ermine::RevoluteJoint::ClearLimits()
{
	RevoluteJointHandle->EnableLimit(false);
}

glm::vec2 Ermine::RevoluteJoint::GetAnchorALocLocalSpace()
{
	glm::vec2 AnchorALocalPosition = glm::vec2(RevoluteJointHandle->GetLocalAnchorA().x, RevoluteJointHandle->GetLocalAnchorA().y);
	return Ermine::vertexWorldToPixels(AnchorALocalPosition);
}

glm::vec2 Ermine::RevoluteJoint::GetAnchorBLocLocalSpace()
{
	glm::vec2 AnchorBLocalPosition = glm::vec2(RevoluteJointHandle->GetLocalAnchorB().x, RevoluteJointHandle->GetLocalAnchorB().y);
	return Ermine::vertexWorldToPixels(AnchorBLocalPosition);
}

glm::vec2 Ermine::RevoluteJoint::GetAnchorALocPixelSpace()
{
	glm::vec2 AnchorAWorldPosition = glm::vec2(RevoluteJointHandle->GetAnchorA().x, RevoluteJointHandle->GetAnchorA().y);
	return Ermine::coordWorldToPixels(AnchorAWorldPosition);
}

glm::vec2 Ermine::RevoluteJoint::GetAnchorBLocPixelSpace()
{
	glm::vec2 AnchorBWorldPosition = glm::vec2(RevoluteJointHandle->GetAnchorB().x, RevoluteJointHandle->GetAnchorB().y);
	return Ermine::coordWorldToPixels(AnchorBWorldPosition);
}


#pragma region MotorFunctions

void Ermine::RevoluteJoint::EnableMotor(bool enabled)
{
	RevoluteJointHandle->EnableMotor(enabled);
}
void Ermine::RevoluteJoint::SetMotorSpeed(float speed)
{
	RevoluteJointHandle->SetMotorSpeed(speed);
}
void Ermine::RevoluteJoint::SetMaxMotorTorque(float torque)
{
	RevoluteJointHandle->SetMaxMotorTorque(torque);
}

bool Ermine::RevoluteJoint::IsMotorEnabled()
{
	return RevoluteJointHandle->IsMotorEnabled();
}
float Ermine::RevoluteJoint::GetMotorSpeed()
{
	return RevoluteJointHandle->GetMotorSpeed();
}
float Ermine::RevoluteJoint::GetMotorTorque()
{
	return RevoluteJointHandle->GetMotorTorque(1.0f / PhysicsWorldTimestep);
}

#pragma endregion MotorFunctions


float Ermine::RevoluteJoint::GetReferenceAngleInDegrees()
{
	return glm::degrees<float>(GetReferenceAngleInRadians());
}
float Ermine::RevoluteJoint::GetReferenceAngleInRadians()
{
	return RevoluteJointHandle->GetReferenceAngle();
}


void Ermine::RevoluteJoint::HelperConstructRevoluteJoint(b2Body* BodyA, b2Body* BodyB, glm::vec2 AnchorAWithRespectToBoxCentre, glm::vec2 AnchorBWithRespectToBoxCentre, float ReferenceAngle, bool ShouldBodiesAttachedByTheJointCollide)
{
	glm::vec2 LocalAnchorAWorldSpace = Ermine::vertexPixelsToWorld(AnchorAWithRespectToBoxCentre);
	glm::vec2 LocalAnchorBWorldSpace = Ermine::vertexPixelsToWorld(AnchorBWithRespectToBoxCentre);

	b2RevoluteJointDef RevDef;

	RevDef.bodyA = BodyA;
	RevDef.bodyB = BodyB;
	RevDef.collideConnected = ShouldBodiesAttachedByTheJointCollide;

	RevDef.localAnchorA = b2Vec2(LocalAnchorAWorldSpace.x, LocalAnchorAWorldSpace.y);
	RevDef.localAnchorB = b2Vec2(LocalAnchorBWorldSpace.x, LocalAnchorBWorldSpace.y);

	RevDef.referenceAngle = ReferenceAngle;

	RevoluteJointHandle = (b2RevoluteJoint*)Ermine::Universum->CreateJoint(&RevDef);
}
