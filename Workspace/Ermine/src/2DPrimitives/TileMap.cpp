#include "stdafx.h"
#include "TileMap.h"

#include "ErmineJsonFunctions.h"

namespace Ermine
{
	TileMap::TileMap(std::filesystem::path TileMapFilePath)
	{
		/*std::ifstream FileRaw(TileMapFilePath);

		nlohmann::json TileSetFile;
		TileSetFile << FileRaw;

		TileMapName = TileSetFile["TileMapName"].dump();
		
		NumberOfTilesWidth = std::stoi(TileSetFile["NumberOfTilesWidth"].dump());
		NumberOfTilesHeight = std::stoi(TileSetFile["NumberOfTilesHeight"].dump());

		//Extract Layer Data
		for (auto i = TileSetFile["TileData"].begin(); i != TileSetFile["TileData"].end(); i++)
		{
			std::string LayerName;
			std::vector<int> LayerData;

			LayerName = i.key();
			LayerData = Ermine::ExtractIntDataFromJsonArray(i.value());

			LayerData.emplace_back(std::make_pair(LayerName, LayerData));
		}

		//Extract TileMap Data..
		std::string StringArray = TileSetFile["TileMap"];*/

		std::ifstream FileRaw(TileMapFilePath);

		nlohmann::json TileSetFile;
		TileSetFile << FileRaw;

		//Start Extracting Layers//
		for (auto i = TileSetFile["Layers"].begin(); i != TileSetFile["Layers"].end(); i++)
		{
			Layer Container;

			Container.Name = i.key();

			Container.TileWidth  = std::stoi((*i)["TileWidthPixels"].dump());
			Container.TileHeight = std::stoi((*i)["TileHeightPixels"].dump());

			Container.NumberOfTilesHorizontal = std::stoi((*i)["NumberOfTilesHorizontal"].dump());
			Container.NumberOfTilesVertical   = std::stoi((*i)["NumberOfTilesVertical"].dump());

			Container.LayerNumber = std::stoi((*i)["LayerNumber"].dump());

			Container.LayerData = ExtractIntDataFromJsonArray((*i)["TileData"].dump());

			Layers.emplace_back(Container);
		}
		//Ended Extracting Layers//

		//Start Extracting TileSets//
		for (auto i = TileSetFile["TileMap"].begin(); i != TileSetFile["TileMap"].end(); i++)
		{
			std::string ExtractedPath = i.key().c_str();

			TileSetsBuffer.emplace_back(new TileSet(std::filesystem::path(ExtractedPath.c_str())));

			TileSetStartIndexTracker.emplace_back(std::stoi(TileSetFile["TileMap"][ExtractedPath.c_str()].dump()));
		}
		//Ended Extracting TileSets//

		std::cout << "HEre";
	}

	TileMap::~TileMap()
	{
		for (int i = 0; i < TileSetsBuffer.size(); i++)
		{
			delete TileSetsBuffer[i];
		}

	}


	TileMap::TileMap(TileMap& rhs)
	{

	}
	TileMap TileMap::operator=(TileMap& rhs)
	{
		return *this;
	}

	TileMap::TileMap(TileMap&& rhs)
	{
	}
	TileMap TileMap::operator=(TileMap&& rhs)
	{
		return *this;
	}


	bool TileMap::operator==(TileMap&& rhs)
	{
		return false;
	}


	int TileMap::GetStartIndex()
	{
		return 0;
	}
	int TileMap::GetEndIndex()
	{
		return 0;
	}

	Ermine::Sprite* TileMap::GetSprite(int Index)
	{
		return nullptr;
	}
}
