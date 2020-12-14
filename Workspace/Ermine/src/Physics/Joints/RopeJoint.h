#pragma once
#include<iostream>
#include<vector>
#include<string>

#include "Physics/Physics.h"
#include "Physics/Joints/JointBase.h"

/*
	This Is Like The Distance Joint But Instead Of Setting The Distance Between The Joints We Set The Maximum Distance Between The Points..
*/

namespace Ermine
{
	class PhysicsComponent2D;
	class RubeLoader;

	class RopeJoint : public JointBase
	{
	protected:
		//There Is Absolutely No Reason For A Default Constructor To Exist..
		RopeJoint() = delete;

		//This Constructor Allows You To Setup Anchor Points Along With Rope Length..
		RopeJoint(std::string JointName,b2Body* BodyA, b2Body* BodyB,glm::vec2 LocalAnchorAPixelCoordinates ,glm::vec2 LocalAnchorBPixelCoordinates , float RopeLengthInPixels = 0.0f, bool CollideConnected = false);

	private:
		//The Reason Why This Is Dangerous Is Because The Outside World Is Responsible For Construction Of Said Joint..
		RopeJoint(b2Joint* Joint,std::string JointName, b2Body* BodyA, b2Body* BodyB);

	public:
		//The Destructor Is Implemted To Dispatch Call To Parent Destructors
		virtual ~RopeJoint() {};

		Ermine::EJointType GetType() override { return Ermine::EJointType::RopeJoint; }

		RopeJoint(const RopeJoint& rhs) = delete;
		RopeJoint& operator=(const RopeJoint& rhs) = delete;

		RopeJoint(RopeJoint&& rhs);
		RopeJoint& operator=(RopeJoint&& rhs);

		static std::shared_ptr<Ermine::RopeJoint> Generate(std::string JointName, b2Body* BodyA, b2Body* BodyB, glm::vec2 LocalAnchorAPixelCoordinates, glm::vec2 LocalAnchorBPixelCoordinates, float RopeLengthInPixels = 0.0f, bool CollideConnected = false);

	private:
		static std::shared_ptr<Ermine::RopeJoint> Generate(b2Joint* Joint, std::string JointName, b2Body* BodyA, b2Body* BodyB);

	public:
		float GetLength();
		void  SetMaxLength(float MaxLength);
		float GetMaxLength();

		//Start Getting Anchor position..//

		glm::vec2 GetBodyALocalAnchorLocation();
		glm::vec2 GetBodyBLocalAnchorLocation();
		//Ended Getting Anchor Position..//

		glm::vec2 GetReactionForce();
		float GetReactionTorque();

	public:

	protected:

	protected:

	private:

	private:
		friend class PhysicsComponent2D;
		friend class RubeLoader;
	};
}