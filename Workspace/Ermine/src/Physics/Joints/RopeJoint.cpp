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
	if (ValidFlag == true)
		return RopeJointHandle->GetLength();
	else return -9999.0f;
}
void Ermine::RopeJoint::SetMaxLength(float MaxLength)
{
	if (ValidFlag == true)
	{
		float MaxLengthWorld = Ermine::scalarPixelsToWorld(MaxLength);
		RopeJointHandle->SetMaxLength(MaxLengthWorld);
	}
}

float Ermine::RopeJoint::GetMaxLength()
{
	if (ValidFlag == true)
	{
		float MaxLengthWorld = Ermine::scalarWorldToPixels(RopeJointHandle->GetMaxLength());
		return MaxLengthWorld;
	}
	else return -9999.0f;
}


glm::vec2 Ermine::RopeJoint::GetBodyALocalAnchorLocation()
{
	if (ValidFlag == true)
	{
		b2Vec2 LocalAnchorLocation = RopeJointHandle->GetLocalAnchorA();
		glm::vec2 LocalAnchorLocPixel = Ermine::vertexWorldToPixels(B2Vec2ToGLM(LocalAnchorLocation));
		return LocalAnchorLocPixel;
	}
	else return glm::vec2(-9999.0f, -9999.0f);
}
glm::vec2 Ermine::RopeJoint::GetBodyBLocalAnchorLocation()
{
	if (ValidFlag == true)
	{
		b2Vec2 LocalAnchorLocation = RopeJointHandle->GetLocalAnchorB();
		glm::vec2 LocalAnchorLocPixel = Ermine::vertexWorldToPixels(B2Vec2ToGLM(LocalAnchorLocation));
		return LocalAnchorLocPixel;
	}
	else return glm::vec2(-9999.0f, -9999.0f);
}

glm::vec2 Ermine::RopeJoint::GetBodyAWorldAnchorLocationPixels()
{
	if (ValidFlag == true)
	{
		b2Vec2 LocalAnchorLocation = RopeJointHandle->GetAnchorA();
		glm::vec2 LocalAnchorLocPixel = Ermine::coordWorldToPixels(B2Vec2ToGLM(LocalAnchorLocation));
		return LocalAnchorLocPixel;
	}
	else return glm::vec2(-9999.0f, -9999.0f);
}
glm::vec2 Ermine::RopeJoint::GetBodyBWorldAnchorLocationPixels()
{
	if (ValidFlag == true)
	{
		b2Vec2 LocalAnchorLocation = RopeJointHandle->GetAnchorB();
		glm::vec2 LocalAnchorLocPixel = Ermine::coordWorldToPixels(B2Vec2ToGLM(LocalAnchorLocation));
		return LocalAnchorLocPixel;
	}
	else return glm::vec2(-9999.0f, -9999.0f);
}


glm::vec2 Ermine::RopeJoint::GetReactionForce()
{
	if (ValidFlag == true)
		return Ermine::B2Vec2ToGLM(RopeJointHandle->GetReactionForce(Ermine::PhysicsWorldTimestep));
	else return glm::vec2(-9999.0f, -9999.0f);
}
float Ermine::RopeJoint::GetReactionTorque()
{
	if (ValidFlag == true)
		return RopeJointHandle->GetReactionTorque(Ermine::PhysicsWorldTimestep);
	else return -9999.0f;
}


void Ermine::RopeJoint::HelperCreateRopeJointHandle(b2Body* BodyA, b2Body* BodyB, glm::vec2 LocalAnchorAPixelCoordinates, glm::vec2 LocalAnchorBPixelCoordinates, float RopeLength, bool CollideConnected)
{
	float RopeLengthWorld = Ermine::scalarPixelsToWorld(RopeLength);

	glm::vec2 LocalAnchorAWorldCoordinates = Ermine::vertexPixelsToWorld(LocalAnchorAPixelCoordinates);
	glm::vec2 LocalAnchorBWorldCoordinates = Ermine::vertexPixelsToWorld(LocalAnchorBPixelCoordinates);

	b2RopeJointDef RDef;

	RDef.bodyA = BodyA;
	RDef.bodyB = BodyB;
	RDef.collideConnected = CollideConnected;

	RDef.localAnchorA = b2Vec2(LocalAnchorAWorldCoordinates.x, LocalAnchorAWorldCoordinates.y);
	RDef.localAnchorB = b2Vec2(LocalAnchorBWorldCoordinates.x, LocalAnchorBWorldCoordinates.y);

	RDef.maxLength = RopeLengthWorld;

	RopeJointHandle = (b2RopeJoint*)Universum->CreateJoint(&RDef);

	JointBase::JointHandle = RopeJointHandle;
}
