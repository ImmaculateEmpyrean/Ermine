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
	class RopeJoint : public JointBase
	{
	protected:
		//There Is Absolutely No Reason For A Default Constructor To Exist..
		RopeJoint() = delete;

		//This Constructor Allows You To Setup Anchor Points Along With Rope Length..
		RopeJoint(std::string JointName,b2Body* BodyA, b2Body* BodyB,glm::vec2 LocalAnchorAPixelCoordinates ,glm::vec2 LocalAnchorBPixelCoordinates , float RopeLengthInPixels = 0.0f, bool CollideConnected = false);

	public:
		//The Destructor Is Implemted To Dispatch Call To Parent Destructors
		virtual ~RopeJoint() {};

		Ermine::EJointType GetType() override { return Ermine::EJointType::RopeJoint; }

		RopeJoint(const RopeJoint& rhs) = delete;
		RopeJoint& operator=(const RopeJoint& rhs) = delete;

		RopeJoint(RopeJoint&& rhs);
		RopeJoint& operator=(RopeJoint&& rhs);

		static std::shared_ptr<Ermine::RopeJoint> Generate(std::string JointName, b2Body* BodyA, b2Body* BodyB, glm::vec2 LocalAnchorAPixelCoordinates, glm::vec2 LocalAnchorBPixelCoordinates, float RopeLengthInPixels = 0.0f, bool CollideConnected = false);

	public:
		float GetLength();
		void  SetMaxLength(float MaxLength);
		float GetMaxLength();

		//Start Getting Anchor position..//

		virtual glm::vec2 GetBodyALocalAnchorLocation() override;
		virtual glm::vec2 GetBodyBLocalAnchorLocation() override;
		//Ended Getting Anchor Position..//

		glm::vec2 GetReactionForce();
		float GetReactionTorque();

	public:

	protected:

	protected:

	private:

	private:


	};
}