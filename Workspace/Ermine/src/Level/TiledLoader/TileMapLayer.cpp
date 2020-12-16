#include "stdafx.h"
#include "TileMapLayer.h"

#include "TiledLoader.h"

namespace Ermine
{
	TileMapLayer::TileMapLayer()
	{}


	std::shared_ptr<Ermine::TileMapLayer> TileMapLayer::Generate()
	{
		return std::shared_ptr<Ermine::TileMapLayer>(new Ermine::TileMapLayer());
	}


	void TileMapLayer::SetLayerName(std::string LayerName)
	{
		Name = LayerName;
	}
	std::string TileMapLayer::GetLayerName()
	{
		return Name;
	}


	int TileMapLayer::GetTileWidth()
	{
		return TileWidth;
	}
	int TileMapLayer::GetTileHeight()
	{
		return TileHeight;
	}
	std::pair<int, int> TileMapLayer::GetTileDiamensions()
	{
		return std::make_pair(TileWidth, TileHeight);
	}


	int TileMapLayer::GetNumberOfTilesWidth()
	{
		return NumberOfTilesWidth;
	}

	int TileMapLayer::GetNumberOfTilesHeight()
	{
		return NumberOfTilesHeight;
	}
	

	int TileMapLayer::GetLayerNumber()
	{
		return LayerNumber;
	}


	std::vector<int> TileMapLayer::GetLayerData()
	{
		return LayerData;
	}
}