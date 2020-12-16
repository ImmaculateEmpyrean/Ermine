#include "stdafx.h"
#include "TiledLoader.h"

#include "EngineResourceHandlers/Manifest.h"

namespace Ermine
{
    static std::filesystem::path GetImagePathRelativeToExecutable(std::filesystem::path PathToConsider)
    {
        std::string PathStr = PathToConsider.u8string();

        auto FilePath = Ermine::Manifest::GetString("Texture");

        if (!FilePath.has_value())
        {
            STDOUTDefaultLog_Error("Manifest File May Probably Be Corrupted As We Cannot Query Key : Texture From It.. Falling Back To Default Location Hardcoded");
        }

        std::string TexturePath = FilePath.value_or("Texture");
        std::size_t found = PathStr.find(TexturePath);

        if (found != std::string::npos)
            PathStr.erase(PathStr.begin(), PathStr.begin() + found);
        else
            STDOUTDefaultLog_Error("Image Set In Rube Editor Might Not Exist In A Subfolder From Exe.. Please Check All The Paths Of The Rube Json File.. Errored Path : %s", PathToConsider.u8string().c_str());

        return std::filesystem::path(PathStr);
    }

    std::shared_ptr<Ermine::TileMap> TiledLoader::Generate(std::filesystem::path TmxFilepath)
    {
        //Populate The TileMap In This Function..
        auto TileMap = Ermine::TileMap::Generate("Test", std::vector<std::shared_ptr<Ermine::TileMapLayer>>());

        tmx::Map map;
        if (map.load(TmxFilepath.u8string()))
        {
            for (auto TileSet : map.getTilesets())
            {
                std::string TileSetName = TileSet.getName();
                
                std::filesystem::path TileSetPath = TileSet.getImagePath();
                TileSetPath = GetImagePathRelativeToExecutable(TileSetPath);
                auto TileSetSprite = Ermine::Sprite::GenerateSprite(TileSetPath);

                std::pair<int, int> TileSize;
                TileSize.first = TileSet.getTileSize().x;
                TileSize.second= TileSet.getTileSize().y;

                int StartIndex = TileSet.getFirstGID();

                std::shared_ptr<Ermine::TileSet> ErTileSet = Ermine::TileSet::Generate(TileSetName, TileSetSprite, TileSize, StartIndex);

                TileMap->AddTileSet(ErTileSet);
            }

            const auto& layers = map.getLayers();

            int LayerNumberCounter = 0;
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
                    auto Layer = Ermine::TileMapLayer::Generate();

                    Layer->NumberOfTilesWidth = tileLayer.getSize().x;
                    Layer->NumberOfTilesHeight= tileLayer.getSize().y;

                    Layer->LayerNumber = LayerNumberCounter++;

                    Layer->TileWidth = 32.0f;
                    Layer->TileHeight = 32.0f;

                    for (auto Tile : tileLayer.getTiles())
                        Layer->LayerData.emplace_back(Tile.ID);
                    
                    TileMap->AddLayerToBack(Layer);
                }
            }
        }

        return TileMap;
    }

}
