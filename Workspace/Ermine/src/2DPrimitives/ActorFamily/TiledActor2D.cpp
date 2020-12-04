#include "stdafx.h"
#include "TiledActor2D.h"

#include <tmxlite/Map.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/ObjectGroup.hpp>

namespace Ermine
{
	TiledActor2D::TiledActor2D(std::filesystem::path TmxFilePath)
		:
		ImageBase()
	{
		tmx::Map map;
		if (map.load("TileMaps/TestMap.tmx"))
		{
			const auto& layers = map.getLayers();
			for (const auto& layer : layers)
			{
				if (layer->getType() == tmx::Layer::Type::Object)
				{
					const auto& objectLayer = layer->getLayerAs<tmx::ObjectGroup>();
					const auto& objects = objectLayer.getObjects();
					for (const auto& object : objects)
					{
						//do stuff with object properties
					}
				}
				else if (layer->getType() == tmx::Layer::Type::Tile)
				{
					const auto& tileLayer = layer->getLayerAs<tmx::TileLayer>();
					//read out tile layer properties etc...
					
					for (auto Tile : tileLayer.getTiles())
					{
						//Read Out The Tiles And Stuff..
						std::cout<<Tile.ID<<std::endl;
					}

				}
			}

			const auto& tilesets = map.getTilesets();
			for (const auto& tileset : tilesets)
			{
				//read out tile set properties, load textures etc...
			}
		}
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


/*		LibTmxparser Code Written.. Might Or Might Not Be Correct
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
					if (TilesetStartGid[i] > tile.gid)
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

		std::cout << "Atleast I Didnt Break :>" << std::endl;*/