#include "stdafx.h"
#include "TileSet.h"

#include "EngineResourceHandlers/GlobalTextureCache.h"

namespace Ermine
{
	TileSet::TileSet(std::filesystem::path TilesetPath)
		:
		TileSetFilePath(TilesetPath)
	{
		HelperConstructorLoadTileMapFromFilePath();
	}

	TileSet::~TileSet()
	{
		for (auto& a : SpritesInTheTileset)
			delete a;
	}


	TileSet::TileSet(const TileSet& rhs)
		:
		TileSetFilePath(rhs.TileSetFilePath)
	{
		HelperConstructorLoadTileMapFromFilePath();
	}

	TileSet TileSet::operator=(const TileSet& rhs)
	{
		TileSetFilePath = rhs.TileSetFilePath;
		
		HelperConstructorLoadTileMapFromFilePath();

		return *this;
	}


	TileSet::TileSet(TileSet&& rhs)
		:
		SpritesInTheTileset(rhs.SpritesInTheTileset),
		TileSetFilePath(rhs.TileSetFilePath),
		TileWidth(rhs.TileWidth),
		TileHeight(rhs.TileHeight)
	{}
	TileSet TileSet::operator=(TileSet&& rhs)
	{
		SpritesInTheTileset = rhs.SpritesInTheTileset;
		TileSetFilePath = rhs.TileSetFilePath;
		TileWidth = rhs.TileWidth;
		TileHeight = rhs.TileHeight;

		return *this;
	}


	bool TileSet::operator==(TileSet& rhs)
	{
		if (TileSetFilePath == rhs.TileSetFilePath)
			return true;
		else return false;
	}


	Sprite* TileSet::GetTile(int index)
	{
		return SpritesInTheTileset[index];
	}

	std::filesystem::path TileSet::GetFilePath()
	{
		return TileSetFilePath;
	}


	void TileSet::HelperConstructorLoadTileMapFromFilePath()
	{
		std::ifstream TileSetFileRaw(TileSetFilePath);

		nlohmann::json TileSetFile;
		TileSetFile << TileSetFileRaw; //Load The JSON Into The JSON Adapter..

		TileSetName = TileSetFile["TileSetName"].dump();
		TileWidth = std::stoi(TileSetFile["TileWidth"].dump());
		TileHeight = std::stoi(TileSetFile["TileHeight"].dump());

		auto TextureHandler = Ermine::GlobalTextureCache::Get();

		std::vector<Ermine::Texture*> TexturesInTheTileSet;
		std::vector<Ermine::TileSet::UV> UVCache;

		for (auto i = TileSetFile["Texture"].begin(); i != TileSetFile["Texture"].end(); i++)
		{
			Texture* ptr = TextureHandler->GetTextureFromFile(i.key());
			TexturesInTheTileSet.emplace_back(ptr);

			TileSet::UV Container = ExtractUVFromJSONArrayString(TileSetFile["Texture"][i.key()].dump());
			UVCache.emplace_back(Container);
		}
		for (int i = 0; i < TexturesInTheTileSet.size(); i++)
		{
			PopulateSpritesContainer(TexturesInTheTileSet[i], UVCache[i]);
		}
	}

	TileSet::UV TileSet::ExtractUVFromJSONArrayString(std::string ArrayString)
	{
		ArrayString.erase(std::remove(ArrayString.begin(), ArrayString.end(), '['), ArrayString.end());
		ArrayString.erase(std::remove(ArrayString.begin(), ArrayString.end(), ']'), ArrayString.end());

		std::string Line;
		std::vector<float> CoordinatesUV;
		for (auto ch : ArrayString)
		{
			if (ch != ',')
				Line.push_back(ch);
			else
			{
				CoordinatesUV.emplace_back(std::stof(Line));
				Line.clear();
			}
		}
		CoordinatesUV.emplace_back(std::stof(Line)); //This Is For THe LAst Coordinate Not Followed By a ,
		Line.clear();

		TileSet::UV Container;
		Container.BottomLeft.x = CoordinatesUV[0];
		Container.BottomLeft.y = CoordinatesUV[1];
		Container.TopRight.x = CoordinatesUV[2];
		Container.TopRight.y = CoordinatesUV[3];

		return Container;
	}


	void TileSet::PopulateSpritesContainer(Texture* txt, UV Container)
	{
		int ImageWidth  = txt->GetWidth();
		int ImageHeight = txt->GetHeight();

		for (int i = 0; i < ImageWidth; i = i+TileWidth)
		{
			for (int j = 0; j < ImageHeight; j = j + TileHeight)
			{
				glm::vec2 TopRight;
				glm::vec2 BottomLeft;

				BottomLeft.x = ((float)i) / ((float)ImageWidth);
				BottomLeft.y = ((float)j) / ((float)ImageHeight);

				TopRight.x = ((float)(i + TileWidth -1)) / ((float)ImageWidth);
				TopRight.y = ((float)(j + TileHeight - 1)) / ((float)ImageHeight);

				if (BottomLeft.x >= Container.BottomLeft.x && BottomLeft.y >= Container.BottomLeft.y)
				{
					if (TopRight.x <= Container.TopRight.x && TopRight.y <= Container.TopRight.y)
					{
						SpritesInTheTileset.emplace_back(new Ermine::Sprite(txt, BottomLeft, TopRight));
					}
				}
			}
		}

	}
}
