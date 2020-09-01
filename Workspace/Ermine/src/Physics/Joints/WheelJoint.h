#pragma once
#include<iostream>
#include<vector>
#include<string>

#include "Physics/Physics.h"
#include "Physics/Joints/JointBase.h"

namespace Ermine
{
	class WheelJoint:public JointBase
	{
	public:
		//There Is Absolutely No Use In Having A Default Line Joint
		WheelJoint() = delete;

		//Think Of This As The Most Default Constructor To test The Joint..
		WheelJoint(b2Body* BodyA, b2Body* BodyB, bool ShouldBodiesCollide = false);

	public:
		virtual b2Joint* GetJoint() override { return WheelJointHandle; }
		virtual operator b2Joint* () override { return WheelJointHandle; }

		virtual glm::vec2 GetBodyALocalAnchorLocation() override;
		virtual glm::vec2 GetBodyBLocalAnchorLocation() override;

		virtual glm::vec2 GetBodyAWorldAnchorLocationPixels() override;
		virtual glm::vec2 GetBodyBWorldAnchorLocationPixels() override;
	protected:

	protected:

	private:

	private:
		b2WheelJoint* WheelJointHandle = nullptr;

	};
}