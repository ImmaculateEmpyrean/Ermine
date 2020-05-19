#include "stdafx.h"
#include "TileMap.h"

#include "ErmineJsonFunctions.h"

namespace Ermine
{
	TileMap::TileMap(std::filesystem::path TileMapFilePath)
		:
		TileMapPath(TileMapFilePath)
	{
		LoadTileMapFromPath();
	}

	TileMap::~TileMap()
	{
		for (int i = 0; i < TileSetsBuffer.size(); i++)
		{
			if(TileSetsBuffer[i] != nullptr) //No Point In Deleting a Nullptr Right..
				delete TileSetsBuffer[i];
		}

	}


	TileMap::TileMap(const TileMap& rhs)
	{
		TileMapPath = rhs.TileMapPath;
		LoadTileMapFromPath();
	}
	TileMap TileMap::operator=(const TileMap& rhs)
	{
		TileMapPath = rhs.TileMapPath;
		LoadTileMapFromPath();

		return *this;
	}

	TileMap::TileMap(TileMap&& rhs)
	{
		TileMapPath = rhs.TileMapPath;
		TileMapName = rhs.TileMapName;

		Layers = std::move(rhs.Layers);

		TileSetsBuffer = std::move(rhs.TileSetsBuffer);

		TileSetStartIndexTracker = rhs.TileSetStartIndexTracker;
	}
	TileMap TileMap::operator=(TileMap&& rhs)
	{
		TileMapPath = rhs.TileMapPath;
		TileMapName = rhs.TileMapName;

		Layers = std::move(rhs.Layers);

		TileSetsBuffer = std::move(rhs.TileSetsBuffer);

		TileSetStartIndexTracker = rhs.TileSetStartIndexTracker;

		return *this;
	}


	bool TileMap::operator==(TileMap&& rhs)
	{
		return TileMapPath == rhs.TileMapPath;
	}


	int TileMap::GetStartIndex()
	{
		return 1;
	}
	int TileMap::GetEndIndex()
	{
		return TileSetStartIndexTracker[TileSetStartIndexTracker.size() - 1] + TileSetsBuffer[TileSetsBuffer.size() - 1]->GetNumberOfSpritesInTileSet();
	}

	Ermine::Sprite* TileMap::GetSprite(int Index)
	{
		//This Still Does Not Work..
		//We WIll Simply Check RAnge

		int c = 0;

		while (true)
		{
			if (Index < TileSetEndIndexTracker[c] && Index >= TileSetStartIndexTracker[c])
				break;
			c++;
		}
		
		return TileSetsBuffer[c]->GetTile(Index);

		/*int c = 0;
		while (Index < TileSetEndIndexTracker[c] || c < TileSetEndIndexTracker.size())
			c++;

		if (c >= TileSetEndIndexTracker.size())
			c--;

		Index = Index - TileSetStartIndexTracker[c];

		return TileSetsBuffer[c]->GetTile(Index); */
	}


	void TileMap::LoadTileMapFromPath()
	{
		//See That The TileMap FilePath Is Not Empty At ANy cost...
		assert(!TileMapPath.empty());

		std::ifstream FileRaw(TileMapPath);

		nlohmann::json TileSetFile;
		TileSetFile << FileRaw;

		TileMapName = TileSetFile["TileMapName"].dump();

		//Start Extracting Layers//
		for (auto i = TileSetFile["Layers"].begin(); i != TileSetFile["Layers"].end(); i++)
		{
			Layer Container;

			Container.Name = i.key();

			Container.TileWidth = std::stoi((*i)["TileWidthPixels"].dump());
			Container.TileHeight = std::stoi((*i)["TileHeightPixels"].dump());

			Container.NumberOfTilesHorizontal = std::stoi((*i)["NumberOfTilesHorizontal"].dump());
			Container.NumberOfTilesVertical = std::stoi((*i)["NumberOfTilesVertical"].dump());

			Container.LayerNumber = std::stoi((*i)["LayerNumber"].dump());

			Container.LayerData = ExtractIntDataFromJsonArray((*i)["TileData"].dump());

			Layers.emplace_back(Container);
		}
		//Ended Extracting Layers//

		//Start Extracting TileSets//
		for (auto i = TileSetFile["TileSet"].begin(); i != TileSetFile["TileSet"].end(); i++)
		{
			std::string ExtractedPath = i.key();

			std::ifstream TileSetFileInputRaw(ExtractedPath);

			nlohmann::json TileSetJsonFile;
			TileSetJsonFile << TileSetFileInputRaw;

			int NumberOfTiles = std::stoi(TileSetJsonFile["NumberOfTiles"].dump());

			TileSetsBuffer.emplace_back(new TileSet(std::filesystem::path(ExtractedPath)));

			TileSetStartIndexTracker.emplace_back(std::stoi(TileSetFile["TileSet"][ExtractedPath.c_str()]["StartIndex"].dump()));
			TileSetEndIndexTracker.emplace_back(NumberOfTiles + TileSetStartIndexTracker[TileSetStartIndexTracker.size()-1]);
		}
		//Ended Extracting TileSets//
	}
}
