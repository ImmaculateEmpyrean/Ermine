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
		virtual void Draw();
	public:

	protected:

	protected:

	private:

	private:
		std::vector<Ermine::Texture*> AllTexturesToChoose;
		char* CurrentItemForDisplayInComboBox = nullptr;

		std::vector<bool> TextureSelected;

		int TileWidth = 16;
		int TileHeight = 16;

		float UvBottomLeft[2] = { 0.0f,0.0f };
		float UvTopRight[2] = { 1.0f,1.0f };

		char* NameBuffer;
	};
}