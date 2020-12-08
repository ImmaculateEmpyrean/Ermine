#include "stdafx.h"
#include "Level.h"

#include "nlohmann/json.hpp"
#include "ErmineJsonFunctions.h"

#include "2DPrimitives/ActorFamily/Actor2D.h"
#include "2DPrimitives/ActorFamily/PhysicsActor2D.h"

#include "Level/RubeLoader.h"

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

	for (auto& i : Package.Components)
		ActorBuffer.emplace_back(Ermine::PhysicsActor2D::Generate(Ermine::Sprite::GetNullSprite(), std::move(i)));
	//Ended Getting And Adding Physics Based Actors//
	

	/*//Start Get All Actor2D Declared In The Level//
	for (auto& i : LevelJson["Actor2D"].items())
	{
		auto j = i.value();

		std::string Name = j["Name"].dump();
		Name.erase(std::remove(Name.begin(), Name.end(), '"'), Name.end());

		std::string TexturePath = j["Texture"].dump();
		TexturePath.erase(std::remove(TexturePath.begin(), TexturePath.end(), '"'), TexturePath.end());

		std::string SpawnLocation = j["Location"].dump();
		SpawnLocation.erase(std::remove(SpawnLocation.begin(), SpawnLocation.end(), '"'), SpawnLocation.end());
		auto SpawnLocationInt = Ermine::ExtractIntDataFromJsonArray(SpawnLocation);
		glm::vec2 SpawnLoc = glm::vec2(SpawnLocationInt[0], SpawnLocationInt[1]);

		//This Is Not Being USed Right Now..
		std::string RecieveEvents = j["RecievingEvents"].dump();
		RecieveEvents.erase(std::remove(RecieveEvents.begin(), RecieveEvents.end(), '"'), RecieveEvents.end());
		std::vector<int> RecieveEventsInt = Ermine::ExtractIntDataFromJsonArray(RecieveEvents);

		//Start Generate An Add An Actor2D Inside Ermine Level//
		std::shared_ptr<Ermine::Actor2D> Act = Ermine::Actor2D::GenerateActor2D(std::filesystem::path(TexturePath), SpawnLoc);

		//Add The Generated Actor After It Was Processed By The Callback Into The Buffer.
		ActorBuffer.emplace_back(Actor2DConstructionCallback(std::move(Act)));
	}
	//Ended Get All Actor2D Declared In The Level//*/
}
