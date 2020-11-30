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
	protected:
		//There Is Absolutely No Use In Having A Default Line Joint
		WheelJoint() = delete;

		//This Constructs Cinstructs The Joint Using The Most Appropriate Defaults As Set By One Of The Box2D Examples.. Not Recommended..
		WheelJoint(std::string Name, b2Body* BodyA, b2Body* BodyB, glm::vec2 LocalAnchorA, glm::vec2 LocalAnchorB, glm::vec2 LocalTranslationalAxisInBodyA, bool CollideConnected = false);

		//Think Of This As The Most Default Constructor To test The Joint.. 
		WheelJoint(std::string Name, b2Body* BodyA, b2Body* BodyB, b2WheelJointDef Def);

	public:

		//Virtual Destructor To Properly Deallocate JointBase
		virtual ~WheelJoint();

		virtual EJointType GetType() override { return Ermine::EJointType::WheelJoint; }

		WheelJoint(const WheelJoint& rhs) = delete;
		WheelJoint& operator=(const WheelJoint& rhs) = delete;

		WheelJoint(WheelJoint&& rhs);
		WheelJoint& operator=(WheelJoint&& rhs);

		//Start Generate Functions..//
		static std::shared_ptr<Ermine::WheelJoint> Generate(std::string Name, b2Body* BodyA, b2Body* BodyB, glm::vec2 LocalAnchorA, glm::vec2 LocalAnchorB, glm::vec2 LocalTranslationalAxisInBodyA, bool CollideConnected = false);
		static std::shared_ptr<Ermine::WheelJoint> Generate(std::string Name, b2Body* BodyA, b2Body* BodyB, b2WheelJointDef Def);
		//Ended Generate Functions..//
	public:
		

		virtual glm::vec2 GetBodyALocalAnchorLocation() override;
		virtual glm::vec2 GetBodyBLocalAnchorLocation() override;

		
	protected:

	protected:

	private:

	private:

	};
}