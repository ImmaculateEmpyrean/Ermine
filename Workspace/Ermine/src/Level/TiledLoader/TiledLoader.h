#pragma once
#include<iostream>
#include<vector>
#include<string>

#include<fstream>
#include<filesystem>

#include <tmxlite/Map.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/ObjectGroup.hpp>

#include "TileMap.h"
#include "TileSet.h"

namespace Ermine
{
	class TiledLoader
	{
	public:
		static std::shared_ptr<Ermine::TileMap> Generate(std::filesystem::path TmxFilepath);
	};
}