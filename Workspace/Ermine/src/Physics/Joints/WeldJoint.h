#pragma once
#include<iostream>
#include<vector>
#include<string>

#include "Physics/Physics.h"
#include "Physics/Joints/JointBase.h"

namespace Ermine
{
	class PhysicsComponent2D;
	class RubeLoader;

	class WeldJoint : public JointBase
	{
	protected:
		//There Is Absolutely No Reason To Have An Empty Weld Joint..
		WeldJoint() = delete;

		//This Constructor Is To Be Used To Set Up Anchor Points All Coordinates In Pixel Space
		WeldJoint(std::string JointName,b2Body* BodyA, b2Body* BodyB, glm::vec2 AnchorAWithRespectToBoxCentre, glm::vec2 AnchorBWithRespectToBoxCentre, bool ShouldBodiesAttachedByTheJointCollide = false);

	private:
		WeldJoint(b2Joint* Pointer ,std::string JointName, b2Body* BodyA, b2Body* BodyB);

	public:
		virtual ~WeldJoint() override;

		Ermine::EJointType GetType() override { return Ermine::EJointType::WeldJoint; }

		WeldJoint(const WeldJoint& rhs) = delete;
		WeldJoint operator=(const WeldJoint& rhs) = delete;

		WeldJoint(WeldJoint&& rhs);
		WeldJoint& operator=(WeldJoint&& rhs);

		static std::shared_ptr<Ermine::WeldJoint> Generate(std::string JointName, b2Body* BodyA, b2Body* BodyB, glm::vec2 AnchorAWithRespectToBoxCentre, glm::vec2 AnchorBWithRespectToBoxCentre, bool ShouldBodiesAttachedByTheJointCollide = false);

	private:
		//Beware While Constructing The Joint..
		static std::shared_ptr<Ermine::WeldJoint> Generate(b2Joint* Joint,std::string JointName, b2Body* BodyA, b2Body* BodyB);

	public:
		glm::vec2 GetBodyALocalAnchorLocation();
		glm::vec2 GetBodyBLocalAnchorLocation();

	public:

	protected:

	protected:

	private:
		
	private:

		friend class PhysicsComponent2D;
		friend class RubeLoader;
	};
}