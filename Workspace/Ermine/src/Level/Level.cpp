#include "stdafx.h"
#include "Level.h"

#include "nlohmann/json.hpp"
#include "ErmineJsonFunctions.h"

#include "2DPrimitives/ActorFamily/Actor2D.h"
#include "2DPrimitives/ActorFamily/PhysicsActor2D.h"

#include "RubeLoader/RubeLoader.h"

Ermine::Level::Level(std::filesystem::path LevelPath)
	:
	FilePath(LevelPath)
{}

Ermine::Level::Level(Level&& rhs)
	:
	Object(rhs)
{
	auto ForeignLock = rhs.GetObjectMutex();
	auto Lock = GetObjectMutex();

	FilePath = std::move(rhs.FilePath);

	WorldGravity = std::move(rhs.WorldGravity);

	ActorBuffer = std::move(rhs.ActorBuffer);
}

Ermine::Level& Ermine::Level::operator=(Level&& rhs)
{
	auto ForeignLock = rhs.GetObjectMutex();
	auto Lock = GetObjectMutex();

	Object::operator=(std::move(rhs));

	FilePath = std::move(rhs.FilePath);

	WorldGravity = std::move(rhs.WorldGravity);

	ActorBuffer = std::move(rhs.ActorBuffer);

	return *this;
}

std::vector<std::shared_ptr<Ermine::Actor2DBase>> Ermine::Level::GetActors()
{
	auto Lock = GetObjectMutex();

	return ActorBuffer;
}

glm::vec2 Ermine::Level::GetGravityVector()
{
	return WorldGravity;
}

void Ermine::Level::LoadLevel()
{
	std::ifstream FileRaw("Levels\\TestLevel.json");

	nlohmann::json LevelJson;
	FileRaw >> LevelJson;

	//Start Get And Add All Actors To The Scene//
	for (auto Actor2DDef : LevelJson["Actor2D"].items())
	{
		std::filesystem::path TexturePath;
		glm::vec2 SpawnLocation(0.0f);

		for (auto Actor2DParameters : Actor2DDef.value().items())
		{
			if (Actor2DParameters.key() == "Texture")
			{
				std::string TexturePathStr = Actor2DParameters.value().dump();
				TexturePathStr.erase(std::remove(TexturePathStr.begin(), TexturePathStr.end(), '"'), TexturePathStr.end());
				TexturePath = TexturePathStr;
			}

			if (Actor2DParameters.key() == "Location")
			{
				std::string SpawnLocationStr = Actor2DParameters.value().dump();
				SpawnLocationStr.erase(std::remove(SpawnLocationStr.begin(), SpawnLocationStr.end(), '"'), SpawnLocationStr.end());
				auto SpawnLocationInt = Ermine::ExtractIntDataFromJsonArray(SpawnLocationStr);
				SpawnLocation = glm::vec2(SpawnLocationInt[0], SpawnLocationInt[1]);
			}
		}
		//Start Generate An Add An Actor2D Inside Ermine Level//
		std::shared_ptr<Ermine::Actor2D> Act = Ermine::Actor2D::GenerateActor2D(std::filesystem::path(TexturePath), SpawnLocation);

		//Add The Generated Actor After It Was Processed By The Callback Into The Buffer.
		ActorBuffer.emplace_back(Actor2DConstructionCallback(std::move(Act)));
	}
	//Ended Get And Add All Actors To The Scene// 

	//Start Getting And Adding Physics Based Actors//
	std::string PhysicsPath = LevelJson["PhysicsScene"].dump();
	PhysicsPath.erase(std::remove(PhysicsPath.begin(), PhysicsPath.end(), '"'), PhysicsPath.end());

	auto Package = Ermine::RubeLoader::ReadFile(PhysicsPath);

	//Start Adding PhysicsActors To The ActorBuffer..//

	//Three For Loops Were Used As The First Actors To Be Drawn Are The Ones Not In Said Container Package.RenderOrder
	std::unordered_map<unsigned int, bool> TallyActorGeneration;
	for (auto& i : Package.RenderOrder)
	{
		if (Package.Sprites.find(i.second) != Package.Sprites.end())
			TallyActorGeneration[i.second] = true;
	}

	for (int i = 0; i < Package.Components.size(); i++)
	{
		if(TallyActorGeneration.find(i) == TallyActorGeneration.end())
			ActorBuffer.emplace_back(Ermine::PhysicsActor2D::Generate(Ermine::Sprite::GetNullSprite(), Package.Components[i]));
	}

	for (auto& i : Package.RenderOrder)
	{
		if (Package.Sprites.find(i.second) != Package.Sprites.end())
			ActorBuffer.emplace_back(Ermine::PhysicsActor2D::Generate(Package.Sprites[i.second], Package.Components[i.second]));
		
	}
	//Ended Adding PhysicsActors To The ActorBuffer..//
}
