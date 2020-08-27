#include "stdafx.h"
#include "JointBase.h"

Ermine::JointBase::JointBase(b2Body* BodyA, b2Body* BodyB, bool CollisionEnableBetweenTheBoduesWhichShareTheJoint)
{
}

Ermine::JointBase::~JointBase()
{
}

Ermine::JointBase::JointBase(JointBase&& rhs)
{
}

Ermine::JointBase& Ermine::JointBase::operator=(JointBase&& rhs)
{
    return *this;
}



unsigned int Ermine::JointBase::GetUniqueIdentifier()
{
    return 0;
}
