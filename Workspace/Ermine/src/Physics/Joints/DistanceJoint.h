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
	protected:
		//There Is Absolutely No Point in An Empty Joint Existing..
		DistanceJoint() = delete;

		//This Constructor Is To Be Used To Set Up Anchor Points All Coordinates In Pixel Space
		DistanceJoint(std::string JointName,b2Body* BodyA, b2Body* BodyB, glm::vec2 AnchorAWithRespectToBoxCentre,glm::vec2 AnchorBWithRespectToBoxCentre, bool ShouldBodiesAttachedByTheJointCollide = false);

	public:
		//This Destructor Is An Override From JointBAse
		virtual ~DistanceJoint() override;

		//Get The Concrete Type Of The Joint In Question..
		virtual Ermine::EJointType GetType() override { return Ermine::EJointType::DistenceJoint; };

		//How Do You Even Copy A Joint ?
		DistanceJoint(const DistanceJoint& rhs) = delete;
		DistanceJoint& operator=(const DistanceJoint& rhs) = delete;

		//Well You Could Move A Joint
		DistanceJoint(DistanceJoint&& rhs);
		DistanceJoint& operator=(DistanceJoint&& rhs);

		static std::shared_ptr<Ermine::DistanceJoint> Generate(std::string JointName, b2Body* BodyA, b2Body* BodyB, glm::vec2 AnchorAWithRespectToBoxCentre, glm::vec2 AnchorBWithRespectToBoxCentre, bool ShouldBodiesAttachedByTheJointCollide = false);

	public:
		virtual glm::vec2 GetBodyALocalAnchorLocation() override;
		virtual glm::vec2 GetBodyBLocalAnchorLocation() override;

		float GetLength();
		void SetLength(float Length);

		float GetFrequency();
		void SetFrequency(float Frequency);

		float GetDampingRatio();
		void  SetDampingRatio(float Ratio);
	public:

	protected:

	protected:

	private:

	private:
	

	};
}