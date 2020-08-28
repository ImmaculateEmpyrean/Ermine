#include "stdafx.h"
#include "DistanceJoint.h"

namespace Ermine
{
	Ermine::DistanceJoint::DistanceJoint(b2Body* BodyA, b2Body* BodyB, bool ShouldBodiesAttachedByTheJointCollide)
		:
		JointBase(BodyA, BodyB)
	{
		ConstructDistanceJointHelper(BodyA, BodyB, glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), ShouldBodiesAttachedByTheJointCollide);
	}
	DistanceJoint::DistanceJoint(b2Body* BodyA, b2Body* BodyB, glm::vec2 AnchorAWithRespectToBoxCentre, bool ShouldBodiesAttachedByTheJointCollide)
		:
		JointBase(BodyA, BodyB)
	{
		ConstructDistanceJointHelper(BodyA, BodyB, AnchorAWithRespectToBoxCentre, glm::vec2(0.0f, 0.0f), ShouldBodiesAttachedByTheJointCollide);
	}
	DistanceJoint::DistanceJoint(b2Body* BodyA, b2Body* BodyB, glm::vec2 AnchorAWithRespectToBoxCentre, glm::vec2 AnchorBWithRespectToBoxCentre, bool ShouldBodiesAttachedByTheJointCollide)
		:
		JointBase(BodyA, BodyB)
	{
		ConstructDistanceJointHelper(BodyA, BodyB, AnchorAWithRespectToBoxCentre ,AnchorBWithRespectToBoxCentre, ShouldBodiesAttachedByTheJointCollide);
	}

	DistanceJoint::~DistanceJoint()
	{
		if (DistanceJointHandle != nullptr)
		{
			Universum->DestroyJoint(DistanceJointHandle);
			DistanceJointHandle = nullptr;
		}
	}


	void DistanceJoint::ConstructDistanceJointHelper(b2Body* BodyA, b2Body* BodyB, glm::vec2 AnchorAWithRespectToBoxCentre, glm::vec2 AnchorBWithRespectToBoxCentre, bool ShouldBodiesAttachedByTheJointCollide)
	{
		glm::vec2 LocalAnchorAWorldSpace = Ermine::vertexPixelsToWorld(AnchorAWithRespectToBoxCentre);
		glm::vec2 LocalAnchorBWorldSpace = Ermine::vertexPixelsToWorld(AnchorBWithRespectToBoxCentre);

		b2DistanceJointDef DisDef;

		DisDef.bodyA = BodyA;
		DisDef.bodyB = BodyB;
		DisDef.collideConnected = ShouldBodiesAttachedByTheJointCollide;

		DisDef.localAnchorA = b2Vec2(LocalAnchorAWorldSpace.x,LocalAnchorAWorldSpace.y);
		DisDef.localAnchorB = b2Vec2(LocalAnchorBWorldSpace.x,LocalAnchorBWorldSpace.y);

		DistanceJointHandle = (b2DistanceJoint*)Ermine::Universum->CreateJoint(&DisDef);
	}
}