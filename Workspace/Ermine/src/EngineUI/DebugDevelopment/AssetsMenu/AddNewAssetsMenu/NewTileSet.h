#pragma once
#include<iostream>
#include<vector>
#include<string>

#include "Graphics/Renderer/MaterialSystem/Texture.h"
#include "EngineUI/PrecursorWindow.h"

#include "glm.hpp"

namespace Ermine
{
	class AddNewTileSetWindow : public PrecursorWindow
	{
	public:
		AddNewTileSetWindow();
		~AddNewTileSetWindow();

	public:
		virtual void Draw() override;

		AddNewTileSetWindow(const AddNewTileSetWindow& rhs);
		AddNewTileSetWindow operator=(const AddNewTileSetWindow& rhs);

		AddNewTileSetWindow(AddNewTileSetWindow&& rhs);
		AddNewTileSetWindow operator=(AddNewTileSetWindow&& rhs);

	public:

	protected:

	protected:

	private:
		void WriteTileSetToFile();
		
		//This Method Also Copies Some Stuff Into Current Item For DisplayIn Combo box.. A Custom Message To Be Shown To The User.. :>
		void InitBuffers();

		void CopyHelper(const AddNewTileSetWindow& rhs);
		void MoveHelper(AddNewTileSetWindow&& rhs);

	private:
		std::vector<std::shared_ptr<Texture>> AllTexturesToChoose;
		char* CurrentItemForDisplayInComboBox = nullptr;

		std::vector<bool> TextureSelected;

		int TileWidth = 16;
		int TileHeight = 16;

		float UvBottomLeft[2] = { 0.0f,0.0f };
		float UvTopRight[2] = { 1.0f,1.0f };

		char* NameBuffer = nullptr;
		bool MakeFilePathUsingName = true;
		char* FilePathBuffer = nullptr;
	};
}