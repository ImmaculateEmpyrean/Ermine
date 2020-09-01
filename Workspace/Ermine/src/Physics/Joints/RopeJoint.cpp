#include "stdafx.h"
#include "RopeJoint.h"

Ermine::RopeJoint::RopeJoint(b2Body* BodyA, b2Body* BodyB, bool CollideConnected)
	:
	JointBase(BodyA,BodyB)
{
	HelperCreateRopeJointHandle(BodyA, BodyB, glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), 0.0f, CollideConnected);
}

Ermine::RopeJoint::RopeJoint(b2Body* BodyA, b2Body* BodyB, float RopeLength, bool CollideConnected)
	:
	JointBase(BodyA, BodyB)
{
	HelperCreateRopeJointHandle(BodyA, BodyB, glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), RopeLength, CollideConnected);
}

Ermine::RopeJoint::RopeJoint(b2Body* BodyA, b2Body* BodyB, glm::vec2 LocalAnchorAPixelCoordinates, glm::vec2 LocalAnchorBPixelCoordinates, float RopeLength, bool CollideConnected)
	:
	JointBase(BodyA, BodyB)
{
	HelperCreateRopeJointHandle(BodyA, BodyB,LocalAnchorAPixelCoordinates, LocalAnchorBPixelCoordinates, RopeLength, CollideConnected);
}


float Ermine::RopeJoint::GetLength()
{
	return RopeJointHandle->GetLength();
}
void Ermine::RopeJoint::SetMaxLength(float MaxLength)
{
	float MaxLengthWorld = Ermine::scalarPixelsToWorld(MaxLength);
	RopeJointHandle->SetMaxLength(MaxLengthWorld);
}

float Ermine::RopeJoint::GetMaxLength()
{
	float MaxLengthWorld = Ermine::scalarWorldToPixels(RopeJointHandle->GetMaxLength());
	return MaxLengthWorld;
}


glm::vec2 Ermine::RopeJoint::GetAnchorAPixelCoordinates()
{
	glm::vec2 AnchorAWorldLocation = Ermine::B2Vec2ToGLM(RopeJointHandle->GetAnchorA());
	return Ermine::coordWorldToPixels(AnchorAWorldLocation);
}
glm::vec2 Ermine::RopeJoint::GetAnchorBPixelCoordinates()
{
	glm::vec2 AnchorBWorldLocation = Ermine::B2Vec2ToGLM(RopeJointHandle->GetAnchorB());
	return Ermine::coordWorldToPixels(AnchorBWorldLocation);
}

glm::vec2 Ermine::RopeJoint::GetAnchorALocalCoordinates()
{
	glm::vec2 AnchorALocalLocation = Ermine::B2Vec2ToGLM(RopeJointHandle->GetLocalAnchorA());
	return Ermine::vertexWorldToPixels(AnchorALocalLocation);
}
glm::vec2 Ermine::RopeJoint::GetAnchorBLocalCoordinates()
{
	glm::vec2 AnchorBLocalLocation = Ermine::B2Vec2ToGLM(RopeJointHandle->GetLocalAnchorB());
	return Ermine::vertexWorldToPixels(AnchorBLocalLocation);
}


glm::vec2 Ermine::RopeJoint::GetReactionForce()
{
	return Ermine::B2Vec2ToGLM(RopeJointHandle->GetReactionForce(Ermine::PhysicsWorldTimestep));
}
float Ermine::RopeJoint::GetReactionTorque()
{
	return RopeJointHandle->GetReactionTorque(Ermine::PhysicsWorldTimestep);
}


void Ermine::RopeJoint::HelperCreateRopeJointHandle(b2Body* BodyA, b2Body* BodyB, glm::vec2 LocalAnchorAPixelCoordinates, glm::vec2 LocalAnchorBPixelCoordinates, float RopeLength, bool CollideConnected)
{
	glm::vec2 LocalAnchorAWorldCoordinates = Ermine::vertexPixelsToWorld(LocalAnchorAPixelCoordinates);
	glm::vec2 LocalAnchorBWorldCoordinates = Ermine::vertexPixelsToWorld(LocalAnchorBPixelCoordinates);

	b2RopeJointDef RDef;

	RDef.bodyA = BodyA;
	RDef.bodyB = BodyB;
	RDef.collideConnected = CollideConnected;

	RDef.localAnchorA = b2Vec2(LocalAnchorAWorldCoordinates.x, LocalAnchorAWorldCoordinates.y);
	RDef.localAnchorB = b2Vec2(LocalAnchorBWorldCoordinates.x, LocalAnchorBWorldCoordinates.y);

	RDef.maxLength = RopeLength;

	RopeJointHandle = (b2RopeJoint*)Universum->CreateJoint(&RDef);
}
