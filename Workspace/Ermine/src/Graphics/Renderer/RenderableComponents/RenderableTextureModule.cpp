#include "stdafx.h"
#include "RenderableTextureModule.h"

#include "EventSystem/Components/BroadcastComponent.h"

namespace Ermine
{
#pragma region Constructors
    RenderableTextureModule::RenderableTextureModule(std::shared_ptr<Ermine::Actor2DBase> Ptr, std::vector<std::shared_ptr<Ermine::Texture>> Textures)
        :
        Renderable2D(Ptr),
        TexturesBuffer(Textures)
    {}


    RenderableTextureModule::RenderableTextureModule(RenderableTextureModule & rhs)
        :
        Renderable2D(rhs)
    {
        auto ForeignLock = rhs.GetObjectMutex();
        auto Lock = GetObjectMutex();

        TexturesBuffer = rhs.TexturesBuffer;
    }
    RenderableTextureModule& RenderableTextureModule::operator=(RenderableTextureModule& rhs)
    {
        auto ForeignLock = rhs.GetObjectMutex();
        auto Lock = GetObjectMutex();

        Renderable2D::operator=(rhs);

        TexturesBuffer = rhs.TexturesBuffer;

        return *this;
    }

    RenderableTextureModule::RenderableTextureModule(RenderableTextureModule&& rhs)
        :
        Renderable2D(std::move(rhs))
    {
        auto ForeignLock = rhs.GetObjectMutex();
        auto Lock = GetObjectMutex();

        TexturesBuffer = std::move(rhs.TexturesBuffer);
    }
    RenderableTextureModule& RenderableTextureModule::operator=(RenderableTextureModule&& rhs)
    {
        auto ForeignLock = rhs.GetObjectMutex();
        auto Lock = GetObjectMutex();

        Renderable2D::operator=(std::move(rhs));

        TexturesBuffer = std::move(rhs.TexturesBuffer);

        return *this;
    }
#pragma endregion

    void RenderableTextureModule::Initialize()
    {
        //Get The Lock Since We Are Planning On Accessing The Data Contained Within..
        auto Lock = GetObjectMutex();
        
        //Initialize The Renderable2D..
        Renderable2D::Initialize();

        //TexturesBuffer.erase(TexturesBuffer.begin() + 1); //Testing
        //Submit The Texture If The Actor In Question Implements Image Base Atleast..
        /*if (std::dynamic_pointer_cast<Ermine::ImageBase>(Renderable2D::GetBoundActor()))
        {
            for(int i=0;i< std::dynamic_pointer_cast<Ermine::ImageBase>(Renderable2D::GetBoundActor())->GetSpriteBuffer().size();i++)
                SubmitTexture(std::dynamic_pointer_cast<Ermine::ImageBase>(Renderable2D::GetBoundActor())->GetSprite(i)->GetTexture());
        }*/
    }

    void RenderableTextureModule::Refresh(float DeltaTime)
    {
        auto Lock = GetObjectMutex();

        if (GetObjectInitialized() == true)
        {
            Renderable2D::Refresh(DeltaTime);
        }
    }

    std::unique_ptr<Ermine::RenderableTextureModule> RenderableTextureModule::Generate(std::shared_ptr<Actor2DBase> Act)
    {
        if (std::dynamic_pointer_cast<Ermine::ImageBase, Ermine::Actor2DBase>(Act))
        {
            auto ImgActor = std::dynamic_pointer_cast<Ermine::ImageBase, Ermine::Actor2DBase>(Act);

            std::vector <std::shared_ptr<Ermine::Texture>> Textures;
            for(auto i : ImgActor->GetSpriteBuffer())
                Textures.emplace_back(i->GetTexture());

            //new is used here since constructor is most probably protected.. not optimal but easy workaround
            std::unique_ptr<RenderableTextureModule> Module(new RenderableTextureModule(Act,Textures));

            std::shared_ptr<void*> st = std::make_shared<void*>();
            Ermine::BroadcastComponent::BroadcastEvent(std::make_unique<Ermine::OnBeginEvent>(st));

            return Module;
        }
        else
        {
            //Why Should I Return A Renderable Texture Module When There Is No Image Base Implemnted..
            return nullptr;
        }
    }


    void RenderableTextureModule::SubmitTexture(std::filesystem::path TexturePath)
    {
        auto TextureCache = Ermine::GlobalTextureCache::Get();
        TexturesBuffer.emplace_back(TextureCache->GetTextureFromFile(TexturePath));
    }
    void RenderableTextureModule::SubmitTexture(std::shared_ptr<Texture> Texture)
    {
        TexturesBuffer.emplace_back(Texture);
    }

    std::vector<int> RenderableTextureModule::BindTexturesContained()
    {
        std::vector<int> BoundVector;
        BoundVector.resize(TexturesBuffer.size(), 0);
        
        auto TextureCacheGlobal = Ermine::GlobalTextureCache::Get();

        int c = 0;
        for (std::shared_ptr<Texture>& i : TexturesBuffer)
        {
            int BoundSlot = TextureCacheGlobal->Bind(i);
            BoundVector[c++] = BoundSlot;
        }
        return BoundVector;
    }


    void RenderableTextureModule::Clear()
    {
        Renderable2D::Clear();
        TexturesBuffer.clear();
    }
    void RenderableTextureModule::ClearTextureBuffer()
    {
        TexturesBuffer.clear();
    }
}
