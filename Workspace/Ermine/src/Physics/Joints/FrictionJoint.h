#pragma once
#include<iostream>
#include<vector>
#include<string>

#include "Physics/Physics.h"
#include "JointBase.h"

namespace Ermine
{
	//Forward Declaration So That They Can be Made Friends As We Implicitly Trust Them..
	class PhysicsComponent2D;
	class RubeLoader;

	class FrictionJoint :public JointBase
	{
	protected:
		//There Is Ansolutely No Point In An Empty Joint Existing..
		FrictionJoint() = delete;

		/*
		*	Friction Joint Can Only Be Constructed Using Rube For Now..
		*/

	private:
		//This Constructor Is Very Dangerous As It Assumes That The Pointer IS Constructed Properly..
		FrictionJoint(b2Joint* Pointer, std::string JointName, b2Body* BodyA, b2Body* BodyB);

	public:
		//A Destructor Is Necessary As This Class Manages Memory..
		virtual ~FrictionJoint() override;

		//Get The Concrete Type Of The Joint In Question..
		virtual Ermine::EJointType GetType() override { return Ermine::EJointType::FrictionJoint; };

		FrictionJoint(const FrictionJoint& rhs) = delete;
		FrictionJoint& operator=(const FrictionJoint& rhs) = delete;

		FrictionJoint(FrictionJoint&& rhs);
		FrictionJoint& operator=(FrictionJoint&& rhs);

		static std::shared_ptr<Ermine::FrictionJoint> Generate(std::string JointName, b2Body* BodyA, b2Body* BodyB, glm::vec2 AnchorAWithRespectToBoxCentre, glm::vec2 AnchorBWithRespectToBoxCentre, float ReferenceAngleDegrees, glm::vec2 SlidingAxis, bool ShouldBodiesAttachedByTheJointCollide = false);

	private:
		static std::shared_ptr<Ermine::FrictionJoint> Generate(b2Joint* Pointer, std::string JointName, b2Body* BodyA, b2Body* BodyB);

	public:
		
	public:

	protected:

	protected:

	private:

	private:


		friend class Ermine::RubeLoader;
		friend class Ermine::PhysicsComponent2D;
	};
}
