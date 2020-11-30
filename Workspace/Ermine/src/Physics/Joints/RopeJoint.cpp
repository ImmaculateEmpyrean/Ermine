#include "stdafx.h"
#include "RopeJoint.h"

Ermine::RopeJoint::RopeJoint(std::string JointName,b2Body* BodyA, b2Body* BodyB, glm::vec2 LocalAnchorAPixelCoordinates, glm::vec2 LocalAnchorBPixelCoordinates, float RopeLength, bool CollideConnected)
	:
	JointBase(JointName,BodyA, BodyB)
{
	float RopeLengthWorld = Ermine::scalarPixelsToWorld(RopeLength);

	b2Vec2 LocalAnchorA = Ermine::GLMToB2Vec2(Ermine::vertexPixelsToWorld(LocalAnchorAPixelCoordinates));
	b2Vec2 LocalAnchorB = Ermine::GLMToB2Vec2(Ermine::vertexPixelsToWorld(LocalAnchorBPixelCoordinates);

	b2RopeJointDef RDef;

	RDef.bodyA = BodyA;
	RDef.bodyB = BodyB;
	RDef.collideConnected = CollideConnected;

	RDef.localAnchorA = LocalAnchorA;
	RDef.localAnchorB = LocalAnchorB;

	RDef.maxLength = RopeLengthWorld;

	JointHandle = (b2RopeJoint*)Universum->CreateJoint(&RDef);
}

Ermine::RopeJoint::RopeJoint(RopeJoint&& rhs)
	:
	JointBase(std::move(rhs))
{}

Ermine::RopeJoint& Ermine::RopeJoint::operator=(RopeJoint&& rhs)
{
	JointBase::operator=(std::move(rhs));
	return *this;
}

std::shared_ptr<Ermine::RopeJoint> Ermine::RopeJoint::Generate(std::string JointName, b2Body* BodyA, b2Body* BodyB, glm::vec2 LocalAnchorAPixelCoordinates, glm::vec2 LocalAnchorBPixelCoordinates, float RopeLengthInPixels, bool CollideConnected)
{
	std::shared_ptr<Ermine::RopeJoint> RJ(new Ermine::RopeJoint(JointName, BodyA, BodyB, LocalAnchorAPixelCoordinates, LocalAnchorBPixelCoordinates, RopeLengthInPixels, CollideConnected), Ermine::JointDeleter<Ermine::RopeJoint>());
	return RJ;
}

float Ermine::RopeJoint::GetLength()
{
	if (JointBase::GetHealth() == Ermine::JointHealthEnum::StatusOk)
		return scalarWorldToPixels(((b2RopeJoint*)JointHandle)->GetLength());
	else
	{
		STDOUTDefaultLog_Error("Cannot Get Length Of The Revolute Joint As The Health Is Not Okay");
		return -9999.0f;
	}
}
void Ermine::RopeJoint::SetMaxLength(float MaxLength)
{
	if (JointBase::GetHealth() == Ermine::JointHealthEnum::StatusOk)
	{
		float MaxLengthWorld = Ermine::scalarPixelsToWorld(MaxLength);
		((b2RopeJoint*)JointHandle)->SetMaxLength(MaxLengthWorld);
	}
	else STDOUTDefaultLog_Error("Cannot Set MaxLength Of The Revolute Joint As The Health Is Not Okay");
}

float Ermine::RopeJoint::GetMaxLength()
{
	if (JointBase::GetHealth() == Ermine::JointHealthEnum::StatusOk)
	{
		float MaxLengthWorld = Ermine::scalarWorldToPixels(((b2RopeJoint*)JointHandle)->GetMaxLength());
		return MaxLengthWorld;
	}
	else
	{
		STDOUTDefaultLog_Error("Cannot Get MaxLength Of The Revolute Joint As The Health Is Not Okay");
		return -9999.0f;
	}
}


glm::vec2 Ermine::RopeJoint::GetBodyALocalAnchorLocation()
{
	if (JointBase::GetHealth() == Ermine::JointHealthEnum::StatusOk)
	{
		b2Vec2 LocalAnchorLocation = ((b2RopeJoint*)JointHandle)->GetLocalAnchorA();
		glm::vec2 LocalAnchorLocPixel = Ermine::vertexWorldToPixels(B2Vec2ToGLM(LocalAnchorLocation));
		return LocalAnchorLocPixel;
	}
	else
	{
		STDOUTDefaultLog_Error("Cannot Get BodyALocalAnchor Location Of The Revolute Joint As The Health Is Not Okay");
		return glm::vec2(-9999.0f, -9999.0f);
	}
}
glm::vec2 Ermine::RopeJoint::GetBodyBLocalAnchorLocation()
{
	if (JointBase::GetHealth() == Ermine::JointHealthEnum::StatusOk)
	{
		b2Vec2 LocalAnchorLocation = ((b2RopeJoint*)JointHandle)->GetLocalAnchorB();
		glm::vec2 LocalAnchorLocPixel = Ermine::vertexWorldToPixels(B2Vec2ToGLM(LocalAnchorLocation));
		return LocalAnchorLocPixel;
	}
	else
	{
		STDOUTDefaultLog_Error("Cannot Get BodyBLocalAnchor Location Of The Revolute Joint As The Health Is Not Okay");
		return glm::vec2(-9999.0f, -9999.0f);
	}
}


glm::vec2 Ermine::RopeJoint::GetReactionForce()
{
	if (JointBase::GetHealth() == Ermine::JointHealthEnum::StatusOk)
		return Ermine::B2Vec2ToGLM(((b2RopeJoint*)JointHandle)->GetReactionForce(Ermine::PhysicsWorldTimestep));
	else
	{
		STDOUTDefaultLog_Error("Cannot Get ReactionForce Of The Revolute Joint As The Health Is Not Okay");
		return glm::vec2(-9999.0f, -9999.0f);
	}
}
float Ermine::RopeJoint::GetReactionTorque()
{
	if (JointBase::GetHealth() == Ermine::JointHealthEnum::StatusOk)
		return ((b2RopeJoint*)JointHandle)->GetReactionTorque(Ermine::PhysicsWorldTimestep);
	else
	{
		STDOUTDefaultLog_Error("Cannot Get ReactionTorque Of The Revolute Joint As The Health Is Not Okay");
		return -9999.0f;
	}
}
