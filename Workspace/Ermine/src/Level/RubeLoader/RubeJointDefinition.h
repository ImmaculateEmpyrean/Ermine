#pragma once
#include<iostream>
#include<vector>
#include<string>

#include <nlohmann/json.hpp>

#include "Physics/Physics.h"
#include "Physics/PhysicsComponent2D.h"

#include "RubeLoaderPackage.h"

namespace Ermine
{
	class RubeLoader;

	class RubeJointDefinition
	{
	public:
		//No Need For An Empty Object Ever Right
		RubeJointDefinition() = delete;

		//Construct A RubeJointDefinition Using A Joint json an query its members later
		RubeJointDefinition(nlohmann::json Joint,RubeLoaderPackage& Package);

	public:
		
	public:

	protected:

	protected:

	private:

	public:
		//Start Get Anchors//
		b2Vec2 AnchorA   = { 0.0f,0.0f }; 
		b2Vec2 AnchorB   = { 0.0f,0.0f }; 
		//Ended Get Anchors//

		//Start Get Bodies//
		std::shared_ptr<Ermine::PhysicsComponent2D> BodyA = nullptr; 
		std::shared_ptr<Ermine::PhysicsComponent2D> BodyB = nullptr; 
		//Ended Get Bodies//

		//Start Get Flags//
		bool CollideConnected = false;
		bool EnableMotor = false; 
		bool EnableLimit = false; 
		//Ended Get Flags//

		//Start Get Limits And Lengths//
		float Length	 = 0.0f;
		float MaxLength  = 0.0f;
		float UpperLimit = 0.0f;
		float LowerLimit = 0.0f;
		//Ended Get Limits And Lengths//

		//Start Get Motor Options//
		float MotorSpeed = 0.0f; 
		float MaxMotorTorque = 0.0f; 
		//Ended Get Motor Options//

		//Start Get Name//
		std::string JointName = "None";
		std::string JointType = "None";
		//Ended Get Name
		
		//Start Get Misc//
		float ReferenceAngle = 0.0f;
		float Damping = 0.0f;
		float Frequency = 0.0f;
		b2Vec2 LocalAxis = { 0.0f,0.0f };
		//Ended Get Misc//
		

		friend class Ermine::RubeLoader;
	};
}