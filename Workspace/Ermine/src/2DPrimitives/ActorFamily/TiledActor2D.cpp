#include "stdafx.h"
#include "TiledActor2D.h"

namespace Ermine
{
	TiledActor2D::TiledActor2D(std::filesystem::path TmxFilePath)
		:
		ImageBase()
	{
		tmxparser::TmxMap map;
		tmxparser::TmxReturn error = tmxparser::parseFromFile("TileMaps/TestMap.tmx", &map, "");// "TileMaps/ZeldaTileSets.tsx");

		if (!error)
		{
			STDOUTDefaultLog_Trace(map.version);
		}
		else
			STDOUTDefaultLog_Critical("Error Using tmxParser");

		std::vector<std::vector<std::shared_ptr<Ermine::Sprite>>> Tilesets;
		std::vector<unsigned int> TilesetStartGid;
		for (auto Tileset : map.tilesetCollection)
		{
			int columns = Tileset.colCount;
			int rows = Tileset.rowCount;

			auto image = Tileset.image;
			int Spacing = Tileset.tileSpacingInImage;

			int width = image.width;
			int height = image.height;

			int TileWidth = Tileset.tileWidth;
			int TileHeight = Tileset.tileHeight;

			std::filesystem::path TilesetImagePath = image.source;

			int WidthCounter   = Tileset.tileMarginInImage;
			int HeightCounter  = Tileset.tileMarginInImage;

			Tilesets.emplace_back();
			TilesetStartGid.emplace_back(Tileset.firstgid);

			for (int i = 0; i < columns; i++)
			{
				for (int j = 0; j < rows; j++)
				{
					glm::vec2 BottomLeft = glm::vec2({height - TileHeight - HeightCounter,WidthCounter});
					glm::vec2 TopRight = glm::vec2({height - HeightCounter,WidthCounter + TileWidth});

					WidthCounter = WidthCounter + TileWidth + Spacing;
					
					std::shared_ptr<Ermine::Sprite> Tile = Ermine::Sprite::GenerateSprite(TilesetImagePath, BottomLeft, TopRight);
					Tilesets[Tilesets.size() -1].emplace_back(Tile);
				}
				HeightCounter = HeightCounter + TileHeight + Spacing;
				WidthCounter = Tileset.tileMarginInImage;
			}
		}

		std::vector<std::vector<std::shared_ptr<Ermine::Sprite>>> Map;

		for (auto layer : map.layerCollection)
		{
			std::vector<std::shared_ptr<Ermine::Sprite>> Layer;
			for (auto tile : layer.tiles)
			{
				if (tile.gid == 0)
					continue;

				int TileSetIndex = 0;
				bool Found = false;

				for (int i = 0; i < TilesetStartGid.size(); i++)
				{
					if (TilesetStartGid[i] <= tile.gid)
					{
						TileSetIndex = i-1;
						Found = true;
						break;
					}
				}
				
				if (Found == false)
					TileSetIndex = Tilesets.size() - 1;

				int TileIndex = tile.gid - TilesetStartGid[TileSetIndex];
				Layer.emplace_back(Tilesets[TileSetIndex][TileIndex]);
			}
			Map.emplace_back(Layer);
		}

		std::cout << "Atleast I Didnt Break :>" << std::endl;
	}
	glm::vec2 TiledActor2D::GetScreenLocation()
	{
		return glm::vec2();
	}
	glm::mat4 TiledActor2D::GetModelMatrix()
	{
		return glm::mat4();
	}
	std::vector<float> TiledActor2D::GenerateModelSpaceVertexBuffer()
	{
		return std::vector<float>();
	}
	std::vector<Ermine::VertexAttribPointerSpecification> TiledActor2D::GetVertexArraySpecification()
	{
		return std::vector<Ermine::VertexAttribPointerSpecification>();
	}
	std::vector<uint32_t> TiledActor2D::GenerateModelSpaceIndices()
	{
		return std::vector<uint32_t>();
	}
}
