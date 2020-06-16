#pragma once
#include<iostream>
#include<vector>
#include<string>

#include<fstream>
#include<filesystem>

#include <nlohmann/json.hpp>
#include "TileSet.h"

#include "Graphics/Renderer/RendererPrimitives/RendererPrimitives2D/TileMapRendererPrimitive.h"

namespace Ermine
{
	class Renderer2D; //Forward Declaration..

	class TileMap
	{
	public:
		TileMap() = delete; //There is No Reason To Hav An Empty TileMap..
		TileMap(std::filesystem::path TileMapFilePath);

		~TileMap();

	public:
		TileMap(const TileMap& rhs);
		TileMap operator=(const TileMap& rhs);

		TileMap(TileMap&& rhs);
		TileMap operator=(TileMap&& rhs);

		//This is Mostly Crap It Means Nothing To CompareFilePaths..
		bool operator==(TileMap&& rhs);

		//Data Start Index.. We Know Its Not 0.. For Now All TileSets Start At 1 Making This Function Redundant..
		int GetStartIndex(); 
		int GetEndIndex();

		std::shared_ptr<Sprite> GetSprite(int Index);

	public:

	protected:

	protected:

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

		public:
			//This Compares LayerNumbers Only
			bool operator<(const Layer& rhs){ return LayerNumber < rhs.LayerNumber;	}

			//This Compares LayerNumbers Only
			bool operator>(const Layer& rhs) { return LayerNumber > rhs.LayerNumber; }
			
			//This Compares LayerNumbers Only
			bool operator<=(const Layer& rhs) { return LayerNumber <= rhs.LayerNumber; }

			//This Compares LayerNumbers Only
			bool operator>=(const Layer& rhs) { return LayerNumber >= rhs.LayerNumber; }

			//This Compares LayerNumbers Only
			bool operator==(const Layer& rhs) { return LayerNumber == rhs.LayerNumber; }
		};

	private:
		//This Function Is Only Meant For Friends And As Such Is Hidden Away In Private..
		std::vector<Layer> GetAllLayers() { return Layers; } 

		//Set The TileMapPath Variable Before Calling This Method Otherwise It Wont Work..
		void LoadTileMapFromPath();
		void CreateRendererFriendlyDrawable();
		std::pair<VertexArray, std::unordered_map<std::filesystem::path, float>> CreateVertexArrayForLayer(Ermine::TileMap::Layer& layer);
	
	private:
		std::filesystem::path TileMapPath;

		std::string TileMapName;
		
		std::vector<Layer> Layers;

		//A TileMap Is Responsible For Its TileSets And Is Said To Own A TileSet.. This Is Arranged In Increasing Order Of Start Indexes
		std::vector<TileSet*> TileSetsBuffer;

		//This Vector Is Used To Map TileMap Index To TileSetIndex..
		std::vector<int> TileSetStartIndexTracker;
		//This Vector Is Used To Map TileMap Index To TileSetIndex..
		std::vector<int> TileSetEndIndexTracker;

		//This Variable is used by the renderer to draw the Tilemap in question
		Ermine::TileMapRendererPrimitive RendererFriendlyDrawable;
		int NumberOfGridsInXOnScreen = 10; //Please Include Functions For This In The Api In The Near Future..
		int NumberOfGridsInYOnScreen = 10; //Please Include Functions For This In The Api In The Near Future..

		friend class Ermine::Renderer2D;
	};
}