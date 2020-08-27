#pragma once
#include<iostream>
#include<vector>
#include<string>

#include "JointBase.h"

namespace Ermine
{
	class DistanceJoint :public JointBase
	{
	public:
		//There Is Absolutely No Point in An Empty Joint Existing..
		DistanceJoint() = delete;

		//This Is More Or Less The Default Constructor You Are To Use.. 
		DistanceJoint(b2Body* BodyA, b2Body* BodyB, bool ShouldBodiesAttachedByTheJointCollide = false);

	public:
		virtual b2Joint* GetJoint() override { return DistanceJointHandle; }
		virtual operator b2Joint* () override { return DistanceJointHandle;}

	public:

	protected:

	protected:

	private:

	private:
		b2DistanceJoint* DistanceJointHandle = nullptr;

	};
}