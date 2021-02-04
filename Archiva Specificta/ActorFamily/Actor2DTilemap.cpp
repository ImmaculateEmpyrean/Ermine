#include "stdafx.h"
#include "Actor2DTilemap.h"

#include "2DPrimitives/Constructs/Quad.h"
#include "2DPrimitives/Constructs/VertexTextured.h"

#include "Level/TiledLoader/TiledLoader.h" 

namespace Ermine
{
    Actor2DTilemap::Actor2DTilemap(std::shared_ptr<Ermine::TileMap> Map)
        :
        Actor2DShape(),
        TileMap(Map)
    {
        auto TileSets = Map->GetTilesetBuffer();

        for (auto Set : TileSets)
            ImageBase::SetSprite(Set->GetTileSetImage());    
    }


    std::shared_ptr<Ermine::Actor2DTilemap> Actor2DTilemap::Generate(std::filesystem::path TileMapFile)
    {
        std::shared_ptr<Ermine::TileMap> Map = Ermine::TiledLoader::Generate(TileMapFile);
        std::shared_ptr<Ermine::Actor2DTilemap> TM(new Ermine::Actor2DTilemap(Map));

        return TM;
    }
    std::shared_ptr<Ermine::Actor2DTilemap> Actor2DTilemap::Generate(std::shared_ptr<Ermine::TileMap> Map)
    {
        std::shared_ptr<Ermine::Actor2DTilemap> TM(new Ermine::Actor2DTilemap(Map));

        return TM;
    }


    std::vector<float> Actor2DTilemap::GenerateModelSpaceVertexBuffer()
    {
        std::vector<float> ModelCoordinates;

        std::vector<std::shared_ptr<Ermine::TileMapLayer>> Layers = TileMap->GetLayers();

        for (int LayerNumber = 0; LayerNumber < Layers.size(); LayerNumber++)
        {
            int TileWidth = Layers[LayerNumber]->GetTileWidth();
            int TileHeight = Layers[LayerNumber]->GetTileHeight();

            for(int i=0;i<Layers[LayerNumber]->GetNumberOfTilesWidth();i++)
                for (int j = 0; j < Layers[LayerNumber]->GetNumberOfTilesHeight(); j++)
                {
                    if (Layers[LayerNumber]->GetLayerData()[i + (j * Layers[LayerNumber]->GetNumberOfTilesWidth())] == 0)
                        continue;

                    std::shared_ptr<Ermine::Sprite> Spr = TileMap->GetTileSprite(LayerNumber, i, j);
                    //std::shared_ptr<Ermine::Sprite> Spr = Ermine::Sprite::GetNullSprite();

                    Ermine::VertexTextured TopRight;
                    Ermine::VertexTextured BottomRight;
                    Ermine::VertexTextured BottomLeft;
                    Ermine::VertexTextured TopLeft;

                    //Set Position Coordinates..//
                    TopRight.SetPositonCoordinates   ({ (i * TileWidth) + TileWidth,(j * TileHeight),0.0f });
                    BottomRight.SetPositonCoordinates({ (i * TileWidth) + TileWidth,(j * TileHeight) + TileHeight,0.0f });

                    TopLeft.SetPositonCoordinates   ({ (i * TileWidth)             ,(j * TileHeight),0.0f });
                    BottomLeft.SetPositonCoordinates({ (i * TileWidth)             ,(j * TileHeight) + TileHeight,0.0f });
                    //Ended Position Coordinates..//

                    //Start Vertex Color Coordinates..//
                    TopRight.SetVertexColorValue(glm::vec3(1.0f,1.0f,1.0f));
                    TopLeft.SetVertexColorValue (glm::vec3(1.0f,0.0f,0.0f));
                    
                    BottomRight.SetVertexColorValue(glm::vec3(0.0f, 0.0f, 1.0f));
                    TopLeft.SetVertexColorValue    (glm::vec3(0.0f, 1.0f, 0.0f));
                    //Ended Vertex Color Coordinates..//

                    //Start UV Coordinates..//
                    TopRight.SetVertexUV(Spr->GetTopRightUV());
                    BottomLeft.SetVertexUV(Spr->GetBottomLeftUV());

                    TopLeft.SetVertexUV    ({ Spr->GetBottomLeftUV().x,Spr->GetTopRightUV().y  });
                    BottomRight.SetVertexUV({ Spr->GetTopRightUV().x ,Spr->GetBottomLeftUV().y });
                    //Ended UV Coordinates..//
                   
                    //Start Set Texture Number..//
                    int TexNumber = GetTextureNumber(Spr->GetTexture()->GetFilePath());
                    TopRight.SetTextureNumber   (TexNumber);
                    BottomLeft.SetTextureNumber (TexNumber);

                    TopLeft.SetTextureNumber    (TexNumber);
                    BottomRight.SetTextureNumber(TexNumber);
                    //Ended Set TextureNumber..//

                    ModelCoordinates = ModelCoordinates + TopRight;
                    ModelCoordinates = ModelCoordinates + BottomRight;
                    ModelCoordinates = ModelCoordinates + BottomLeft;
                    ModelCoordinates = ModelCoordinates + TopLeft;
                }
        }

        NumberOfVertices = ModelCoordinates.size();
        return ModelCoordinates;
    }

    std::vector<uint32_t> Actor2DTilemap::GenerateModelSpaceIndices()
    {
        std::vector<uint32_t> IndexBuffer;

        //0,1.2
        //0,2,3

        for (int i = 0; i < NumberOfVertices; i = i + 4)
        {
            IndexBuffer.emplace_back(i);
            IndexBuffer.emplace_back(i + 1);
            IndexBuffer.emplace_back(i + 2);

            IndexBuffer.emplace_back(i);
            IndexBuffer.emplace_back(i + 2);
            IndexBuffer.emplace_back(i + 3);
        }

        return IndexBuffer;
    }
}
