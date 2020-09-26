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
	public:
		//There Is Absolutely No Reason For A Default Constructor To Exist..
		RopeJoint() = delete;

		//This Is The Default Constructor For Testing..
		RopeJoint(b2Body* BodyA,b2Body* BodyB,bool CollideConnected = false);

		//This Constructor Additionally Takes Rope Length While Attaching The Origins Of The Two Bodies..
		RopeJoint(b2Body* BodyA, b2Body* BodyB, float RopeLength = 0.0f, bool CollideConnected = false);

		//This Constructor Allows You To Setup Anchor Points Along With Rope Length..
		RopeJoint(b2Body* BodyA, b2Body* BodyB,glm::vec2 LocalAnchorAPixelCoordinates ,glm::vec2 LocalAnchorBPixelCoordinates , float RopeLengthInPixels = 0.0f, bool CollideConnected = false);

	public:
		virtual b2Joint* GetJoint() override { return RopeJointHandle; }
		virtual operator b2Joint* () override { return RopeJointHandle; }

		float GetLength();
		void SetMaxLength(float MaxLength);
		float GetMaxLength();

		//Start Getting Anchor position..//

		virtual glm::vec2 GetBodyALocalAnchorLocation() override;
		virtual glm::vec2 GetBodyBLocalAnchorLocation() override;

		virtual glm::vec2 GetBodyAWorldAnchorLocationPixels() override;
		virtual glm::vec2 GetBodyBWorldAnchorLocationPixels() override;

		//Ended Getting Anchor Position..//

		glm::vec2 GetReactionForce();
		float GetReactionTorque();

	public:

	protected:

	protected:

	private:
		void HelperCreateRopeJointHandle(b2Body* BodyA, b2Body* BodyB, glm::vec2 LocalAnchorAPixelCoordinates, glm::vec2 LocalAnchorBPixelCoordinates, float RopeLength, bool CollideConnected);

	private:
		b2RopeJoint* RopeJointHandle = nullptr;
	};
}