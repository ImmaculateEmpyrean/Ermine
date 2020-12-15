#include "stdafx.h"
#include "FrictionJoint.h"

namespace Ermine
{

	FrictionJoint::FrictionJoint(b2Joint* Pointer, std::string JointName, b2Body* BodyA, b2Body* BodyB)
		:
		JointBase(JointName,BodyA,BodyB)
	{
		//This Is A Dangerous Constructor As We Implicitly Assume That The Pointer Is Properly Constructed..
		JointHandle = Pointer;
	}

	FrictionJoint::~FrictionJoint()
	{}


	FrictionJoint::FrictionJoint(FrictionJoint&& rhs)
		:
		JointBase(std::move(rhs))
	{}
	FrictionJoint& FrictionJoint::operator=(FrictionJoint&& rhs)
	{
		JointBase::operator=(std::move(rhs));
		return *this;
	}


	std::shared_ptr<Ermine::FrictionJoint> FrictionJoint::Generate(b2Joint* Pointer, std::string JointName, b2Body* BodyA, b2Body* BodyB)
	{
		std::shared_ptr<Ermine::FrictionJoint> FJ(new FrictionJoint(Pointer, JointName, BodyA, BodyB));
		return FJ;
	}
}
