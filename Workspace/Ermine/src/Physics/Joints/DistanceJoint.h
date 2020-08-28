#pragma once
#include<iostream>
#include<vector>
#include<string>

#include "Physics/Physics.h"
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

		//This Constructor Is To Be Used To Set Up Anchor Points All Coordinates In Pixel Space
		DistanceJoint(b2Body* BodyA, b2Body* BodyB, glm::vec2 AnchorAWithRespectToBoxCentre, bool ShouldBodiesAttachedByTheJointCollide = false);

		//This Constructor Is To Be Used To Set Up Anchor Points All Coordinates In Pixel Space
		DistanceJoint(b2Body* BodyA, b2Body* BodyB, glm::vec2 AnchorAWithRespectToBoxCentre,glm::vec2 AnchorBWithRespectToBoxCentre, bool ShouldBodiesAttachedByTheJointCollide = false);

		//This Destructor Is An Override From JointBAse
		virtual ~DistanceJoint() override;

	public:
		virtual b2Joint* GetJoint() override { return DistanceJointHandle; }
		virtual operator b2Joint* () override { return DistanceJointHandle;}

	public:

	protected:

	protected:

	private:
		void ConstructDistanceJointHelper(b2Body* BodyA, b2Body* BodyB, glm::vec2 AnchorAWithRespectToBoxCentre, glm::vec2 AnchorBWithRespectToBoxCentre, bool ShouldBodiesAttachedByTheJointCollide = false);

	private:
		b2DistanceJoint* DistanceJointHandle = nullptr;

	};
}