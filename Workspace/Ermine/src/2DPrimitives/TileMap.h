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
		TileMap operator=(TileMap& rhs);

		TileMap(TileMap&& rhs);
		TileMap operator=(TileMap&& rhs);

		bool operator==(TileMap&& rhs);

		//Data Start Index.. We Know Its Not 0.. For Now All TileSets Start At 1 Making This Function Redundant..
		int GetStartIndex(); 
		int GetEndIndex();

		Ermine::Sprite* GetSprite(int Index);

	public:

	protected:

	protected:

	private:
		
	private:
		std::string TileMapName;

		int NumberOfTilesWidth;
		int NumberOfTilesHeight;

		//Stores The Layer Name And The Data Associated With It..
		std::pair<std::string, std::vector<int>> LayerData; 

		//A TileMap Is Responsible For Its TileSets And Is Said To Own A TileSet..
		std::vector<TileSet*> TileSetsBuffer;
		//This Vector Is Used To Map TileMap Index To TileSetIndex..
		std::vector<int> TileSetStartIndexTracker;
	};
}