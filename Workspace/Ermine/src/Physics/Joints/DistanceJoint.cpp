#include "stdafx.h"
#include "DistanceJoint.h"

namespace Ermine
{
	Ermine::DistanceJoint::DistanceJoint(b2Body* BodyA, b2Body* BodyB, bool ShouldBodiesAttachedByTheJointCollide)
		:
		JointBase(BodyA, BodyB, ShouldBodiesAttachedByTheJointCollide)
	{
		b2DistanceJointDef DisDef;

		DisDef.bodyA = BodyA;
		DisDef.bodyB = BodyB;
		DisDef.collideConnected = ShouldBodiesAttachedByTheJointCollide;

		DisDef.localAnchorA = b2Vec2(0.0f, 0.0f);
		DisDef.localAnchorB = b2Vec2(0.0f, 0.0f);

		DistanceJointHandle = (b2DistanceJoint*)Ermine::Universum->CreateJoint(&DisDef);
	}
}
