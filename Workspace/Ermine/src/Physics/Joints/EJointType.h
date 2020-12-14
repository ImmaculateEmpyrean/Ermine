#pragma once
#include<iostream>
#include<vector>
#include<string>

//These Enums Are Used By Us To Determine Which Concrete Joint We Are Workig With..
namespace Ermine
{
	enum class EJointType : unsigned int
	{
		Error = 0,
		DistenceJoint = 1,
		PrismaticJoint = 2,
		RevoluteJoint = 3,
		RopeJoint = 4,
		WeldJoint = 5,
		WheelJoint = 6,
		MotorJoint = 7
	};
}