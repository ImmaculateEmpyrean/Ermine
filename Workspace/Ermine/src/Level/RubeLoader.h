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

namespace Ermine
{
	struct RubeLoaderPackage
	{
		b2Vec2 Gravity;

		std::vector<std::shared_ptr<Ermine::PhysicsComponent2D>> Components;
		std::vector<std::shared_ptr<Ermine::JointBase>> Joints;
		std::vector<std::shared_ptr<Ermine::Sprite>> Sprites;
	};

	class RubeLoader
	{
	public:
		//Well This Can Be Constructed Default Since Its Not Doing Much Management Work.
		RubeLoader() = default;

		//This Is The Function Which Is To Be Called If A Rube Json Is To Be Loaded Into Ermine.. Ot Gives Of Am Intermediate Structure Which Can Be Further Processed..
		static RubeLoaderPackage ReadFile(std::filesystem::path RubeJsonFilePath);
	};
}