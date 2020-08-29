#pragma once
#include<iostream>
#include<vector>
#include<string>

#include "Physics/Physics.h"
#include "JointBase.h"

namespace Ermine
{
	class PrismaticJoint :public JointBase
	{
	public:
		//There Is Ansolutely No Point In An Empty Joint Existing..
		PrismaticJoint() = delete;

		//This Is More Or Less The Default Constructir You Are Expected To Use..
		PrismaticJoint(b2Body* BodyA, b2Body* BodyB, bool ShouldBodiesAttachedByTheJointCollide = false);

		//A Destructor Is Necessary As This Class Manages Memory..
		virtual ~PrismaticJoint() override;

	public:
		virtual b2Joint* GetJoint() override { return PrismaticJointHandle; }
		virtual operator b2Joint* () override { return PrismaticJointHandle; }

	public:

	protected:

	protected:

	private:

	private:
		b2PrismaticJoint* PrismaticJointHandle = nullptr;

	};
}
