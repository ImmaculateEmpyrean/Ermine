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
