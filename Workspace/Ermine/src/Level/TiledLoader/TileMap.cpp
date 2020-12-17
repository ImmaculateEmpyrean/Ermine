#include "stdafx.h"
#include "TileMap.h"

#include "EngineResourceHandlers/GlobalTextureCache.h"

namespace Ermine
{
	TileMap::TileMap(std::string Name, std::vector<std::shared_ptr<Ermine::TileMapLayer>> Layers)
		:
		TileMapName(Name),
		Layers(Layers)
	{}

	TileMap::~TileMap()
	{}


	std::shared_ptr<Ermine::TileMap> TileMap::Generate(std::string Name, std::vector<std::shared_ptr<Ermine::TileMapLayer>> Layers)
	{
		return std::shared_ptr<Ermine::TileMap>(new Ermine::TileMap(Name,Layers));
	}


	std::shared_ptr<Ermine::TileMapLayer>& TileMap::GetLayer(int Index)
	{
		if(Index < 0)
			STDOUTDefaultLog_Critical("Index Not Valid TileMap.cpp-- GetLayer() Function")

		if (Index > Layers.size())
		{
			while (Layers.size() < Index)
				Layers.emplace_back(Ermine::TileMapLayer::Generate());
		}

		return Layers[Index];
	}
	std::vector<std::shared_ptr<Ermine::TileMapLayer>> TileMap::GetLayers(int StartIndex, int EndIndex)
	{
		std::vector<std::shared_ptr<Ermine::TileMapLayer>> Vector;
		std::copy( Layers.begin() + StartIndex, Layers.begin() + EndIndex, Vector.begin());

		return Vector;
	}

	std::vector<std::shared_ptr<Ermine::TileMapLayer>> TileMap::GetLayers()
	{
		return Layers;
	}


	void TileMap::AddLayerToBack(std::shared_ptr<Ermine::TileMapLayer> LayerToAdd)
	{
		Layers.emplace_back(LayerToAdd);
	}
	void TileMap::AddLayerToFront(std::shared_ptr<Ermine::TileMapLayer> LayerToAdd)
	{
		Layers.insert(Layers.begin(), LayerToAdd);
	}
	void TileMap::AddLayerAtIndex(int indexNumber, std::shared_ptr<Ermine::TileMapLayer> LayerToAdd)
	{
		Layers.insert(Layers.begin() + indexNumber, LayerToAdd);
	}

	void TileMap::AddLayersToBack(std::vector<std::shared_ptr<Ermine::TileMapLayer>> LayersToAdd)
	{
		Layers.insert(Layers.end() - 1, LayersToAdd.begin(), LayersToAdd.end());
	}
	void TileMap::AddLayersToFront(std::vector<std::shared_ptr<Ermine::TileMapLayer>> LayersToAdd)
	{
		Layers.insert(Layers.begin(), LayersToAdd.begin(), LayersToAdd.end());
	}
	void TileMap::AddLayersFromIndex(int indexNumber, std::vector<std::shared_ptr<Ermine::TileMapLayer>> LayersToAdd)
	{
		Layers.insert(Layers.begin() + indexNumber, LayersToAdd.begin(), LayersToAdd.end());
	}

	void TileMap::DeleteLayerFromTileMap(int LayerNumber)
	{
		Layers.erase(Layers.begin() + LayerNumber);
	}


	std::vector<std::shared_ptr<Ermine::TileSet>>& TileMap::GetTilesetBuffer()
	{
		return TileSetsBuffer;
	}
	void TileMap::AddTileSet(std::shared_ptr<Ermine::TileSet> TileSet)
	{
		TileSetsBuffer.emplace_back(TileSet);
	}
	std::shared_ptr<Ermine::Sprite> TileMap::GetTileSprite(int LayerNumber, int X, int Y)
	{
		std::vector<int> LayerData = Layers[LayerNumber]->LayerData;
		int NumberOfTilesWidth = Layers[LayerNumber]->NumberOfTilesWidth;

		int SpriteData = LayerData[X + (Y * NumberOfTilesWidth)];

		int Count = 0;
		while (TileSetsBuffer[Count]->GetEndIndex() < SpriteData)
			Count++;

		return TileSetsBuffer[Count]->GetTile(SpriteData);
	}
}
