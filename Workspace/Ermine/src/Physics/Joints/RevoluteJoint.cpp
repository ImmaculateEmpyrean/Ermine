#include "stdafx.h"
#include "RevoluteJoint.h"

Ermine::RevoluteJoint::RevoluteJoint(b2Body* BodyA, b2Body* BodyB, bool ShouldBodiesAttachedByTheJointCollide)
	:
	JointBase(BodyA,BodyB)
{
	b2RevoluteJointDef RevDef;

	RevDef.bodyA = BodyA;
	RevDef.bodyB = BodyB;
	RevDef.collideConnected = ShouldBodiesAttachedByTheJointCollide;

	RevDef.localAnchorA = b2Vec2(0.0f,0.0f);
	RevDef.localAnchorB = b2Vec2(0.0f, 0.0f);

	RevoluteJointHandle = (b2RevoluteJoint*)Ermine::Universum->CreateJoint(&RevDef);
}

Ermine::RevoluteJoint::~RevoluteJoint()
{
	if (RevoluteJointHandle != nullptr)
	{
		Ermine::Universum->DestroyJoint(RevoluteJointHandle);
		RevoluteJointHandle = nullptr;
	}
}