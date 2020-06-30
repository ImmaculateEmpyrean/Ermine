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
		void SpecializedNextColumn(int Threshold);

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
		//Start Main Variables
		Ermine::TileMap Map;
		int LayerChosen = 0;
		int SelectedSpriteIndex = 0;

		std::shared_ptr<Ermine::Texture> TransparentTexture;
		//Ended Main Variables

		//Start Flags
		bool OpenTilesetChoosingMenu = false;
		bool DisplayTilesetChoosingMenu = false;

		bool ViewMapJson = false;
		bool DisplayViewMapJson = false;
		
		bool OpenConstructedMapWindow = false;
		bool DisplayConstructedMapWindow = false;

		bool OpenLayerNameInputWindow = false;
		bool DisplayLayerNameInputWindow = false;

		std::atomic<bool> RecieveTileSetSelectedEventsFlag = true;
		//Ended Flags

		//Start Buffers Raw
		char* NameBuffer;
		char* LayerNameBuffer;
		//Ended Buffers Raw		
	};
}