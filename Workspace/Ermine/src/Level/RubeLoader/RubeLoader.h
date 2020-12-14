#pragma once
#include<iostream>
#include<string>
#include<vector>

#include<fstream>
#include<filesystem>

#include "Physics/Physics.h"
#include "Physics/PhysicsComponent2D.h"

#include "Graphics/Renderer/MaterialSystem/Texture.h"
#include "2DPrimitives/Sprite.h"

#include <glm.hpp>

#include <nlohmann/json.hpp>

#include "RubeJointDefinition.h"
#include "RubeLoaderPackage.h"

namespace Ermine
{
	class RubeLoader
	{
	public:
		//Well This Can Be Constructed Default Since Its Not Doing Any Management Work.
		RubeLoader() = default;

	public:
		//This Is The Function Which Is To Be Called If A Rube Json Is To Be Loaded Into Ermine.. Ot Gives Of Am Intermediate Structure Which Can Be Further Processed..
		static RubeLoaderPackage ReadFile(std::filesystem::path RubeJsonFilePath);

	public:

	protected:

	protected:

	private:
		//Helper Construct Joint Functions
		static void ConstructRevoluteJoint(RubeJointDefinition& JointDef);
		static void ConstructDistanceJoint(RubeJointDefinition& JointDef);
		static void ConstructRopeJoint	   (RubeJointDefinition& JointDef);
		static void ConstructWeldJoint	   (RubeJointDefinition& JointDef);
		static void ConstructWheelJoint   (RubeJointDefinition& JointDef);
		static void ConstructMotorJoint   (RubeJointDefinition& JointDef);

	private:


	};
}