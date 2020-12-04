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
			//Initilaize
			std::vector<std::vector<std::shared_ptr<Ermine::Sprite>>> Tilesets;
			
			const auto& tilesets = map.getTilesets();
			for (const auto& tileset : tilesets)
			{
				//read out tile set properties, load textures etc...
				
			}

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


