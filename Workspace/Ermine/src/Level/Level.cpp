#include "stdafx.h"
#include "Level.h"

#include "nlohmann/json.hpp"
#include "ErmineJsonFunctions.h"

#include "2DPrimitives/ActorFamily/Actor2D.h"

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

	//std::cout << LevelJson.dump();

	/*std::string Gravity = LevelJson["Gravity"];
	Gravity.erase(std::remove(Gravity.begin(), Gravity.end(), '"'), Gravity.end());
	std::vector<int> GravityInt = Ermine::ExtractIntDataFromJsonArray(Gravity);
	WorldGravity.x = GravityInt[0];
	WorldGravity.y = GravityInt[1];*/

	//Start Get All Actor2D Declared In The Level//
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
	//Ended Get All Actor2D Declared In The Level//
}
