#pragma once
#include<iostream>
#include<vector>
#include<string>

#include "EngineUI/PrecursorWindow.h"

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
		void InitializeBuffers();
		void HelperCopyTileMapWindow(const NewTileMap& rhs);
		void HelperMoveTileMapWindow(NewTileMap&& rhs);

		//Set Button Colors
		void SetButtonColorGreen();
		void SetButtonColorRed();

		void ClearButtonColor();

	private:
		//Start Main Variables
		std::string TileMapName;
		std::pair<int, int> NumberOfTiles;
		std::pair<int, int> TileDiamensions;
		//Ended Main Variables

		//Start Flags
		bool OpenTilesetChoosingMenu = false;
		bool DisplayTilesetChoosingMenu = false;

		bool ViewMapXmL = false;
		bool DisplayViewMapXml = false;
		
		bool OpenLayerViewWindow = false;
		bool DisplayLayerViewWindow = false;
		//Ended Flags

		//Start Status Variables

		//Ended Status Variables

		//Start Buffers
		char* NameBuffer;
		char* NameBufferCopy;
		//Ended Buffers
	};
}