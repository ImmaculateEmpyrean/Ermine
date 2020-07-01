#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<functional>

#include "glm.hpp"

#include "EngineUI/PrecursorWindow.h"

#include "EventSystem/Event.h"

#include "2DPrimitives/TileSet.h"
#include "2DPrimitives/TileMap.h"

#include "imgui.h"

namespace Ermine
{
	class NewTileMap : public PrecursorWindow
	{
	public:
		NewTileMap() = delete; //The Callable Must Be Initialized If We Are Able To Draw Tileset View Windows On Our Own Voilition
		NewTileMap(std::function<void(std::filesystem::path)> CreateTilesetViewWindowCallable);

		~NewTileMap(); //Deallocate Buffers Using This Destructor
		
		NewTileMap(const NewTileMap& rhs);
		NewTileMap operator=(const NewTileMap& rhs);

		NewTileMap(NewTileMap&& rhs);
		NewTileMap operator=(NewTileMap&& rhs);

	public:
		virtual void Draw() override; //Marked as override as it was overriden From The Base Class

	public:

	protected:

	protected:

	private:
		void DrawLayerNameInputWindow();

		void DrawDisplayConstructedMapWindow();
		void FixDrawingPosition(ImVec2 BasePosition, int NumberToDrawAtOneD, std::pair<int, int> NumberOfTilesInXandY,std::pair<int,int> TileDiamensions);

		void DrawViewMapJsonWindow();
		void DrawSaveMapJsonWindow();

		void DrawLoadTileMapWindow();

		void DrawViewTilesetsInUseWindow();

		//This Function Is Used To Synchronize With The Outside World.. Stuff Like Process Messages and The Like..
		void RecieveTileSelectedEvents(Ermine::Event* EveObj);
		void Update(); //Dunno Still Why To Use This
		
		void InitializeBuffers();
		void HelperCopyTileMapWindow(const NewTileMap& rhs);
		void HelperMoveTileMapWindow(NewTileMap&& rhs);

		//Start Set Button Colors
		void SetButtonColorGreen();
		void SetButtonColorRed();

		void ClearButtonColor();
		//Ended Set Button Colors

	private:
		//Start Callables//
		std::function<void(std::filesystem::path)> CallableCreateTileSetViewWindow;
		//Ended Callables//

		//Start Main Variables
		Ermine::TileMap Map;
		int LayerChosen = 0;
		int SelectedSpriteIndex = 0;

		std::shared_ptr<Ermine::Texture> TransparentTexture;
		std::shared_ptr<Ermine::Texture> JsonFileIcon;
		//Ended Main Variables

		//Start Flags
		bool OpenTilesetChoosingMenu = false;
		bool DisplayTilesetChoosingMenu = false;

		bool OpenViewMapJson = false;
		bool DisplayViewMapJson = false;
		
		bool OpenConstructedMapWindow = false;
		bool DisplayConstructedMapWindow = false;

		bool OpenLayerNameInputWindow = false;
		bool DisplayLayerNameInputWindow = false;

		bool OpenSaveMapJsonWindow = false;
		bool DisplaySaveMapJsonWindow = false;

		bool OpenLoadTileMapWindow = false;
		bool DisplayLoadTileMapWindow = false;

		bool OpenViewTilesetsInUse = false;
		bool DisplayViewTilesetsInUse = false;

		std::atomic<bool> RecieveTileSetSelectedEventsFlag = true;

		int NewLayerCounter = 0; //This Should Be accounted for everywhere in constructors and some such stuff
		//Ended Flags

		//Start Buffers Raw
		char* NameBuffer;
		char* LayerNameBuffer;
		char* FilePathBuffer;

		char* LoadTilesetsPath;
		//Ended Buffers Raw		
	};
}