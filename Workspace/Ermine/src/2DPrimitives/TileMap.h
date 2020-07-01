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
	class NewTileMap; //Forward Declaration..

	class TileMap
	{
	public:
		TileMap(); //Be Very Careful Empty Tilemaps Inside Renderer Can Crash The Program Spectacularly //= delete; //Make A Descision About This By next Session..
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
		int GetStartIndex(int LayerNumber); 
		int GetEndIndex(int LayerNumber);

		std::shared_ptr<Sprite> GetSprite(int Index,int LayerNumber);
		//int GetIndex(std::shared_ptr<Sprite> SpriteToCheck,std::pair<int, int> TileDiamensions);

		//Start Create Tilemaps Api Functions

		void SetTileValue(int LayerNumber,int TileIndex, int TileValue);
		int GetTileValue(int LayerNumber, int TileIndex);

		//Ended Create Tilemaps Api functions

	public:

	protected:

	protected:

	private:
		struct Layer
		{
			Layer(std::string LayerName)
			{
				Name = LayerName;

				TileWidth = 32;
				TileHeight = 64;

				NumberOfTilesHorizontal = 5;
				NumberOfTilesVertical = 5;

				LayerData.resize((NumberOfTilesHorizontal * NumberOfTilesVertical), 0);
			}

			~Layer()
			{
				for (auto& i : TileSetsBuffer)
				{
					delete i;
					i = nullptr;
				}
			}

			Layer(const Layer& rhs);
			Layer operator=(const Layer& rhs);

			Layer(Layer&& rhs);
			Layer operator=(Layer&& rhs);

			std::string Name;

			int TileWidth;
			int TileHeight;

			int NumberOfTilesHorizontal;
			int NumberOfTilesVertical;

			int LayerNumber = -1; //The Default Value.. System Assigns a value if the value is -1..

			std::vector<int> LayerData; //This Can Be left null it will then be populated by 0's accordingly as of the writing it does not do that

			//A TileMap Is Responsible For Its TileSets And Is Said To Own A TileSet.. This Is Arranged In Increasing Order Of Start Indexes
			std::vector<TileSet*> TileSetsBuffer;

			//This Vector Is Used To Map TileMap Index To TileSetIndex..
			std::vector<int> TileSetStartIndexTracker;
			//This Vector Is Used To Map TileMap Index To TileSetIndex..
			std::vector<int> TileSetEndIndexTracker;

		public:
			int GetStartIndex();
			int GetEndIndex();

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

		private:
			void HelperCopyFunction(const Layer& rhs);
			void HelperMoveFunction(Layer&& rhs);
		};

	private:

		///Start Create Tilemap Api..

		//Highest Number is To Be Given
		void AddLayerToBack(Ermine::TileMap::Layer LayerToAdd); 

		//Number 1 is given and all Other Layers Are Pushed Back By one Donot Use This unless absolutely needed..
		void AddLayerToFront(Ermine::TileMap::Layer LayerToAdd); 

		void AddTileset(std::filesystem::path TilesetPath,int LayerNumber);
		void AddTileset(std::unique_ptr<Ermine::TileSet> TilesetToAdd,int LayerNumber);
		void HelperAddTileset(std::unique_ptr<Ermine::TileSet> TilesetToAdd,int LayerNumber);

		std::string GenerateJsonTileMap();
		void WriteTileMapToDisk();
		///Ended Create Tilemap Api...

		//This Function Is Only Meant For Friends And As Such Is Hidden Away In Private..
		std::vector<Layer> GetAllLayers() { return Layers; } 

		//Set The TileMapPath Variable Before Calling This Method Otherwise It Wont Work..
		void LoadTileMapFromPath();

		void CreateRendererFriendlyDrawable();
		std::pair<VertexArray, std::unordered_map<std::filesystem::path, float>> CreateVertexArrayForLayer(Ermine::TileMap::Layer& layer);
		
		bool HelperCheckIfTilesetExists(std::filesystem::path TileSetPath, int LayerNumber);

	private:
		std::filesystem::path TileMapPath;

		std::string TileMapName;
		
		std::vector<Layer> Layers;
			
		

		//This Variable is used by the renderer to draw the Tilemap in question
		Ermine::TileMapRendererPrimitive RendererFriendlyDrawable;
		/*int NumberOfGridsInXOnScreen = 10; //Please Include Functions For This In The Api In The Near Future..
		int NumberOfGridsInYOnScreen = 10; //Please Include Functions For This In The Api In The Near Future..*/

		//Flag Variables..
		bool TilemapFullyFunctional = false; //This Flag Must Be Set To true If The tilemap is to be used by the renderer

		friend class Ermine::Renderer2D;
		friend class Ermine::NewTileMap;
	};
}