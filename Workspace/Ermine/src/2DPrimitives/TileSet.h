#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<memory>

#include<fstream>
#include<filesystem>

#include "nlohmann/json.hpp"
#include "Sprite.h"

#include "glm.hpp"

namespace Ermine
{
	class TileSet
	{
	public:
		TileSet() = delete; //No Point In Having an Empty Tileset
		//Use This Constructor to Load a Tileset From Disk..
		TileSet(std::filesystem::path TilesetPath);

		~TileSet(); //A TileSet Is Responsible For Its Tiles And Will GEt Rid Of Them When Destructing

	public:
		TileSet(const TileSet& rhs);
		TileSet operator=(const TileSet& rhs);

		TileSet(TileSet&& rhs);
		TileSet operator=(TileSet&& rhs);

		//Compares The Two TileSets Filepath..
		bool operator==(TileSet& rhs);

		std::shared_ptr<Sprite> GetTile(int index); //Returns The Tile At That Index..

		std::filesystem::path GetFilePath();

		std::vector<std::shared_ptr<Sprite>> GetSpriteBuffer() { return SpritesInTheTileset; }
		int GetNumberOfSpritesInTileSet() { return SpritesInTheTileset.size(); };

	public:

	protected:

	protected:

	private:
		struct UV
		{
			glm::vec2 BottomLeft;
			glm::vec2 TopRight;
		};

		void HelperConstructorLoadTileMapFromFilePath();

		UV ExtractUVFromJSONArrayString(std::string ArrayString);
		void PopulateSpritesContainer(std::shared_ptr<Texture> txt,UV Container);

	private:
		std::string TileSetName;

		std::vector<std::shared_ptr<Sprite>> SpritesInTheTileset; //The Tileset After Loading Contains Ready To Draw Tiles Anybody Can Merely Request What They Want My Passing In an Index..

		std::filesystem::path TileSetFilePath;
		int TileWidth; 
		int TileHeight;
	};
}