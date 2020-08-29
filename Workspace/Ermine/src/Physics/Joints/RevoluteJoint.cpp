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

Ermine::RevoluteJoint::RevoluteJoint(b2Body* BodyA, b2Body* BodyB, glm::vec2 AnchorAWithRespectToBoxCentre, glm::vec2 AnchorBWithRespectToBoxCentre, float ReferenceAngle, bool ShouldBodiesAttachedByTheJointCollide)
	:
	JointBase(BodyA, BodyB)
{
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

void Ermine::RevoluteJoint::SetLimits()
{
	//TODO Still Has To Implemnted Properly..
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
