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

		~TileMap();

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
		struct Layer
		{
			std::string Name;

			int TileWidth;
			int TileHeight;

			int NumberOfTilesHorizontal;
			int NumberOfTilesVertical;

			int LayerNumber;

			std::vector<int> LayerData;
		};

		std::string TileMapName;
		
		std::vector<Layer> Layers;

		//A TileMap Is Responsible For Its TileSets And Is Said To Own A TileSet..
		std::vector<TileSet*> TileSetsBuffer;
		//This Vector Is Used To Map TileMap Index To TileSetIndex..
		std::vector<int> TileSetStartIndexTracker;
	};
}