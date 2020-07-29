#include "stdafx.h"
#include "RenderableTextureModule.h"

namespace Ermine
{
    RenderableTextureModule::RenderableTextureModule()
        :
        Renderable2D()
    {}
    RenderableTextureModule::RenderableTextureModule(std::vector<std::shared_ptr<Texture>> Tex)
        :
        Renderable2D(),
        TexturesBuffer(Tex)
    {}

    RenderableTextureModule::RenderableTextureModule(VertexArray Vao)
        :
        Renderable2D(Vao)
    {}
    RenderableTextureModule::RenderableTextureModule(VertexArray Vao, Material Mat)
        :
        Renderable2D(Vao,Mat)
    {}

    RenderableTextureModule::RenderableTextureModule(VertexArray Vao, std::vector<std::shared_ptr<Texture>> Tex)
        :
        Renderable2D(Vao),
        TexturesBuffer(Tex)
    {}
    RenderableTextureModule::RenderableTextureModule(Material Mat, std::vector<std::shared_ptr<Texture>> Tex)
        :
        Renderable2D(Mat),
        TexturesBuffer(Tex)
    {}

    RenderableTextureModule::RenderableTextureModule(VertexArray Vao, Material Mat, std::vector<std::shared_ptr<Texture>> Tex)
        :
        Renderable2D(Vao,Mat),
        TexturesBuffer(Tex)
    {}


    void RenderableTextureModule::SubmitTexture(std::filesystem::path TexturePath)
    {
        auto TextureCache = Ermine::GlobalTextureCache::Get();
        TexturesBuffer.emplace_back(TextureCache->GetTextureFromFile(TexturePath));
    }
    void RenderableTextureModule::SubmitTexture(std::shared_ptr<Texture> Texture)
    {
        TexturesBuffer.emplace_back(Texture);
    }

    std::vector<std::shared_ptr<Texture>>& RenderableTextureModule::GetBuffer()
    {
        return TexturesBuffer;
    }


    std::vector<float> RenderableTextureModule::BindTexturesContained()
    {
        Renderable2D::Bind();

        std::vector<float> BoundVector;
        BoundVector.resize(TexturesBuffer.size(), -99.0f);
        
        auto TextureCacheGlobal = Ermine::GlobalTextureCache::Get();

        int c = 0;
        for (std::shared_ptr<Texture>& i : TexturesBuffer)
        {
            int BoundSlot = TextureCacheGlobal->Bind(i);
            BoundVector[c] = BoundSlot;
            c = c + 1;
        }
        return BoundVector;
    }


    void RenderableTextureModule::Clear()
    {
        //Renderable2D::Clear();
        TexturesBuffer.clear();
    }
    void RenderableTextureModule::ClearTextureBuffer()
    {
        TexturesBuffer.clear();
    }

}
