#pragma once
#include<iostream>
#include<vector>
#include<string>

#include<utility>

#include "TileSet.h"
#include "2DPrimitives/Sprite.h"

namespace Ermine
{
	//ForwardDeclared To Make Friends..
	class TiledLoader;
	class TileMap;

	class TileMapLayer
	{
	protected:
		//There Is No Reason For Anyone To Construct A TileMap Layer.. Only To Use It..
		TileMapLayer();

	public:		
	//Generates A Useless Empty TileMap
	private: static std::shared_ptr<Ermine::TileMapLayer> Generate();
	
	public:
	    void SetLayerName(std::string LayerName);
		std::string GetLayerName();

		int GetTileWidth();
		int GetTileHeight();
		std::pair<int, int> GetTileDiamensions();

		int GetNumberOfTilesWidth();
		int GetNumberOfTilesHeight();

		int GetLayerNumber();

		std::vector<int> GetLayerData();

#pragma region Comparission Operators
		//This Compares LayerNumbers Only
		bool operator<(const TileMapLayer& rhs) { return LayerNumber < rhs.LayerNumber; }

		//This Compares LayerNumbers Only
		bool operator>(const TileMapLayer& rhs) { return LayerNumber > rhs.LayerNumber; }

		//This Compares LayerNumbers Only
		bool operator<=(const TileMapLayer& rhs) { return LayerNumber <= rhs.LayerNumber; }

		//This Compares LayerNumbers Only
		bool operator>=(const TileMapLayer& rhs) { return LayerNumber >= rhs.LayerNumber; }

		//This Compares LayerNumbers Only
		bool operator==(const TileMapLayer& rhs) { return LayerNumber == rhs.LayerNumber; }
#pragma endregion

	private:

	private:
		std::string Name;

		int TileWidth;
		int TileHeight;

		int NumberOfTilesWidth;
		int NumberOfTilesHeight;

		int LayerNumber = -1; //The Default Value.. System Assigns a value if the value is -1..

		std::vector<int> LayerData; //This Can Be left null it will then be populated by 0's accordingly as of the writing it does not do that

		friend class Ermine::TiledLoader;
		friend class Ermine::TileMap;
	};
}