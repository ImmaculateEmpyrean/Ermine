#pragma once
#include<iostream>
#include<vector>
#include<string>

#include<fstream>
#include<filesystem>

#include <nlohmann/json.hpp>
#include "TileSet.h"

namespace Ermine
{
	class TileMap
	{
	public:
		TileMap() = delete; //There is No Reason To Hav An Empty TileMap..
		TileMap(std::filesystem::path TileMapFilePath);

	public:
		TileMap(TileMap& rhs);

	public:

	protected:

	protected:

	private:

	private:
		int NumberOfTilesWidth;
		int NumberOfTilesHeight;

		//Stores The Layer Name And The Data Associated With It..
		std::pair<std::string, std::vector<int>> LayerData; 

		//A TileMap Is Responsible For Its TileSets And Is Said To Own A TileSet..
		std::vector<TileSet*> TileSetsBuffer; 
	};
}