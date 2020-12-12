#pragma once
#include<iostream>
#include<string>
#include<vector>

#include<filesystem>

#include "Physics/Physics.h"
#include <nlohmann/json.hpp>

#include "EngineResourceHandlers/Manifest.h"

namespace Ermine
{
	bool CheckIfStringIsTrueLowerCase(std::string String);
	b2Vec2 GetVec2FromJson(nlohmann::json Vec2Json);
	std::vector<b2Vec2> GetVerticesFromRubeJson(nlohmann::json VelocityBlock);
	void removeSubstrs(std::string& string, const std::string& SubstringToRemove);
	std::filesystem::path GetImagePathRelativeToExecutable(std::filesystem::path PathToConsider);
}