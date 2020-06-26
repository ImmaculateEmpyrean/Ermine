#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<unordered_map>

#include<memory>
#include<algorithm>

#include "EngineUI/PrecursorWindow.h"

#include "EngineUI/WindowHandler.h"
#include "TilesetViewer.h"

#include "nlohmann/json.hpp"

namespace Ermine
{
	class TilesetViewWizard :public PrecursorWindow
	{
	public:
		TilesetViewWizard();
		~TilesetViewWizard();

		TilesetViewWizard(const TilesetViewWizard& rhs);
		TilesetViewWizard operator =(const TilesetViewWizard& rhs);

		TilesetViewWizard(TilesetViewWizard&& rhs);
		TilesetViewWizard operator =(TilesetViewWizard&& rhs);

	public:
		virtual void Draw() override;

	public:

	protected:

	protected:

	private:
		bool HelperCheckFileExtensionForJson(std::string FileName);
		bool HelperCheckerIfJsonIsValid(nlohmann::json JsonFile);
		bool HelperShouldIDisplayThisString(std::string StringToDisplay);
		
		void HelperInitializeBuffers();
		void HelperCopyConstructor(const TilesetViewWizard& rhs);

		void HelperMoveConstructor(TilesetViewWizard&& rhs);
	private:
		char* FilterText = nullptr;
		
		char* TilesetPathInputBuffer = nullptr;
		std::filesystem::path TilesetsPath;

		//Start Flags
		bool ChangePathFromWhichTilesetsAreDisplayed = false;
		bool DisplayChangePathFromWhichTilesetsAreDisplayedWindow = false;
		//Ended Flags

	};
}