#pragma once
#include<iostream>
#include<vector>
#include<string>

#include "Physics/Physics.h"
#include "JointBase.h"

namespace Ermine
{
	class RevoluteJoint : public JointBase
	{
	public:
		//No Point In Having A Default Revolute Joint.. The Joint Must Hold Bodies..
		RevoluteJoint() = delete;

		//This Is Something Like The Default Constructor In This Case Use It Most Of The Time
		RevoluteJoint(b2Body* BodyA, b2Body* BodyB, bool ShouldBodiesAttachedByTheJointCollide = false);

		//Destructor To Get Rid Of The Joint After It Is Used Up..
		virtual ~RevoluteJoint() override;

	public:
		virtual b2Joint* GetJoint() override { return RevoluteJointHandle; }
		virtual operator b2Joint* () override { return RevoluteJointHandle; }

	public:

	protected:

	protected:

	private:

	private:
		b2RevoluteJoint* RevoluteJointHandle = nullptr;

	};
}