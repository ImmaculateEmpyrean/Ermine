#include "stdafx.h"
#include "RevoluteJoint.h"

Ermine::RevoluteJoint::RevoluteJoint(b2Body* BodyA, b2Body* BodyB, bool ShouldBodiesAttachedByTheJointCollide)
	:
	JointBase(BodyA,BodyB)
{}

Ermine::RevoluteJoint::~RevoluteJoint()
{}
