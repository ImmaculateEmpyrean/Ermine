#include "stdafx.h"
#include "PrismaticJoint.h"

Ermine::PrismaticJoint::PrismaticJoint(b2Body* BodyA, b2Body* BodyB, bool ShouldBodiesAttachedByTheJointCollide)
	:
	JointBase(BodyA,BodyB)
{
	b2PrismaticJointDef PrDef;

	PrDef.bodyA = BodyA;
	PrDef.bodyB = BodyB;
	PrDef.collideConnected = ShouldBodiesAttachedByTheJointCollide;

	Universum->CreateJoint(&PrDef);
}

Ermine::PrismaticJoint::~PrismaticJoint()
{
	if (PrismaticJointHandle != nullptr)
	{
		Ermine::Universum->DestroyJoint(PrismaticJointHandle);
		PrismaticJointHandle = nullptr;
	}
}
