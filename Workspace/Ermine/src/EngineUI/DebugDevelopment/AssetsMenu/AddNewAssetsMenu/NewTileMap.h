#pragma once
#include<iostream>
#include<vector>
#include<string>

#include "glm.hpp"

#include "EngineUI/PrecursorWindow.h"

#include "EventSystem/Event.h"

#include "2DPrimitives/TileSet.h"
#include "2DPrimitives/TileMap.h"

namespace Ermine
{
	class NewTileMap : public PrecursorWindow
	{
	public:
		NewTileMap();
		~NewTileMap(); //Deallocate Buffers Using This Destructor
		
		//Urgently Requires a copy constructor the raw pointers are going into the destructor when i construct and then push it into the handler
		NewTileMap(const NewTileMap& rhs);
		NewTileMap operator=(const NewTileMap& rhs);

		NewTileMap(NewTileMap&& rhs);
		NewTileMap operator=(NewTileMap&& rhs);

	public:
		virtual void Draw() override; //Marked as override as it was overriden previously

	public:

	protected:

	protected:

	private:
		void DrawLayerNameInputWindow();

		//This Function Is Used To Synchronize With The Outside World.. Stuff Like Process Messages and The Like..
		void Update();
		void RecieveTileSelectedEvents(Ermine::Event* EveObj);

		//std::string GenerateJsonTileMap(); //Moved This Function To Tilemap.h
		//void WriteTilemapToFile(); //Moved This Function To TileMap.h

		//bool HelperCheckBuffersIfTileSetExists(std::filesystem::path TilesetFilesystemPath);

		void InitializeBuffers();
		void HelperCopyTileMapWindow(const NewTileMap& rhs);
		void HelperMoveTileMapWindow(NewTileMap&& rhs);

		//Set Button Colors
		void SetButtonColorGreen();
		void SetButtonColorRed();

		void ClearButtonColor();

	private:
		/*struct Layer
		{
			std::string LayerName;

			glm::vec2 TileDiamensions;
			glm::vec2 NumberOfTiles;

			std::vector<int> TileData;
			
			int LayerNumber;
		};*/ //This Structure Was Already Defined in Tilemap.h we would like to leverage it hopefully.. 

		///Start Main Variables [A New Imperator Comes]
		Ermine::TileMap Map;
		///Ended Main Variables [A New Imperator Comes]

		//Start Main Variables
		/*std::string TileMapName;
		std::vector<Layer> LayersBuffer;

		/*std::pair<int, int> NumberOfTiles;
		std::pair<int, int> TileDiamensions;*/
		//Ended Main Variables

		//Start Main Variables
		int LayerChosen = 0;
		//Ended Main Variables

		//Start Flags
		bool OpenTilesetChoosingMenu = false;
		bool DisplayTilesetChoosingMenu = false;

		bool ViewMapJson = false;
		bool DisplayViewMapJson = false;
		
		bool OpenLayerViewWindow = false;
		bool DisplayLayerViewWindow = false;

		bool OpenLayerNameInputWindow = false;
		bool DisplayLayerNameInputWindow = false;

		std::atomic<bool> RecieveTileSetSelectedEventsFlag = true;
		//Ended Flags

		//Start Status Variables

		//Ended Status Variables

		//Start Buffers Raw
		char* NameBuffer;
		
		char* LayerNameBuffer;
		//char* LayerNameBufferCopy;
		//Ended Buffers Raw

		//Start Internal Self Managed Buffers
		//std::vector<Ermine::TileSet> TilesetsHoldingBuffer;
		//std::unordered_map<std::string, Ermine::TileSet> TilesetsHoldingBuffer; //Path Assumed as a string because then we can avoid writing a hash func
		//std::shared_ptr<Ermine::Sprite> SelectedSprite = nullptr;
		int SelectedSpriteIndex = 0;

		/*std::vector<std::shared_ptr<Ermine::Sprite>> DrawBuffer;*/
		//Ended Internal Self Managed Buffers
	};
}