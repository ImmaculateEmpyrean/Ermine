#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<memory>
#include<utility>

#include<fstream>
#include<filesystem>

#include "2DPrimitives/Sprite.h"


namespace Ermine
{
	class TileSet
	{
	protected:
		//Use This Constructor to Load a Tileset..
		TileSet(std::string TileSetName,std::shared_ptr<Ermine::Sprite> TileSet, std::pair<int,int> TileDiamensions,int StartIndex);

	public:
		//A Destructor Is Not Needed For Now..
		virtual ~TileSet();

	public:
		static std::shared_ptr<Ermine::TileSet> Generate(std::string TileSetName,std::shared_ptr<Ermine::Sprite> TileSet, std::pair<int, int> TileDiamensions,int StartIndex = 0);

	public:
		std::string GetName();
		void SetName(std::string TileSetName);

		std::shared_ptr<Sprite> GetTile(int index);

		std::shared_ptr<Sprite> GetTileSetImage();
		std::vector<std::shared_ptr<Sprite>> GetSpriteBuffer();
		int GetNumberOfSpritesInTileSet();

		std::pair<int, int> GetTileDiamensions();
		int GetTileWidth();
		int GetTileHeight();

		int GetStartIndex();
		int GetEndIndex();
	public:

	protected:

	protected:

	private:
		
	private:
		std::string TileSetName;
		std::vector<std::shared_ptr<Sprite>> SpritesInTheTileset;
		std::shared_ptr<Sprite> TileSetSprite;

		int TileWidth;
		int TileHeight;

		int StartIndex = 0;
		int EndIndex = 0;
	};
}