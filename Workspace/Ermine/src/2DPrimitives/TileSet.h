#pragma once
#include<iostream>
#include<vector>
#include<string>

#include<fstream>
#include<filesystem>

#include "Graphics/Renderer/MaterialSystem/Texture.h"
#include "Sprite.h"

namespace Ermine
{
	class TileSet
	{
	public:
		TileSet() = delete; //No Point In Having an Empty Tileset
		//Use This Constructor to Load a Tileset From Disk..
		TileSet(std::filesystem::path TilesetPath);


	public:
		Sprite* GetTile(int index); //Returns The Tile At That Index..

	public:

	protected:

	protected:

	private:

	private:
		std::vector<Sprite*> SpritesInTheTileset; //The Tileset After Loading Contains Ready To Draw Tiles Anybody Can Merely Request What They Want My Passing In an Index..

		int TileWidth; 
		int TileHeight;

	};
}