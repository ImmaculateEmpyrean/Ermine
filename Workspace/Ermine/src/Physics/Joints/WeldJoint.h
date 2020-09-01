#pragma once
#include<iostream>
#include<vector>
#include<string>

#include "Physics/Physics.h"
#include "Physics/Joints/JointBase.h"

namespace Ermine
{
	class WeldJoint : public JointBase
	{
	public:
		//There Is Absolutely No Reason To Have An Empty Weld Joint..
		WeldJoint() = delete;

		//Use This As The Default Constructor For Testing Purposes..
		WeldJoint(b2Body* BodyA, b2Body* BodyB, bool CollideConnected = false);

		//This Constructor Is To Be Used To Set Up Anchor Points All Coordinates In Pixel Space
		WeldJoint(b2Body* BodyA, b2Body* BodyB, glm::vec2 AnchorAWithRespectToBoxCentre, bool ShouldBodiesAttachedByTheJointCollide = false);

		//This Constructor Is To Be Used To Set Up Anchor Points All Coordinates In Pixel Space
		WeldJoint(b2Body* BodyA, b2Body* BodyB, glm::vec2 AnchorAWithRespectToBoxCentre, glm::vec2 AnchorBWithRespectToBoxCentre, bool ShouldBodiesAttachedByTheJointCollide = false);

		virtual ~WeldJoint() override;

	public:
		virtual b2Joint* GetJoint() override { return WeldJointHandle; }
		virtual operator b2Joint*() override { return WeldJointHandle; }

		virtual glm::vec2 GetBodyALocalAnchorLocation() override;
		virtual glm::vec2 GetBodyBLocalAnchorLocation() override;

		virtual glm::vec2 GetBodyAWorldAnchorLocationPixels() override;
		virtual glm::vec2 GetBodyBWorldAnchorLocationPixels() override;

	public:

	protected:

	protected:

	private:
		void HelperConstructWeldJoint(b2Body* BodyA, b2Body* BodyB, glm::vec2 AnchorAWithRespectToBoxCentre, glm::vec2 AnchorBWithRespectToBoxCentre, bool CollideConnected);
	private:
		b2WeldJoint* WeldJointHandle = nullptr;

	};
}