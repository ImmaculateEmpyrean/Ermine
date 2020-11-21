#include "stdafx.h"
#include "RenderableTextureModule.h"

#include "EventSystem/Components/BroadcastComponent.h"

namespace Ermine
{
#pragma region Constructors
    RenderableTextureModule::RenderableTextureModule(std::shared_ptr<Ermine::Actor2D> Ptr, std::vector<std::shared_ptr<Ermine::Texture>> Textures)
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

        //Submit The Required Texture To Initialize Renderable Texture Module..
        SubmitTexture(std::dynamic_pointer_cast<Ermine::Actor2D>(Renderable2D::GetBoundActor())->GetSprite()->GetTexture());
    }

    void RenderableTextureModule::Refresh()
    {
        auto Lock = GetObjectMutex();

        if (GetObjectInitialized() == true)
        {
            Renderable2D::Refresh();

            if (GetObjectHealth() == ObjectStatus::StatusOk)
            {
                BindTexturesContained();
            }
        }
    }

    std::unique_ptr<Ermine::RenderableTextureModule> RenderableTextureModule::Generate(std::shared_ptr<Actor2DBase> Act)
    {
        auto ActorIdentifier = Act->GetActorFamilyIdentifier();

        if (ActorIdentifier == ActorFamilyIdentifier::Actor2D)
        {
            //Since We Know This Is An Actor2D Cast Into One..
            std::shared_ptr<Ermine::Actor2D> PActor2D = std::dynamic_pointer_cast<Ermine::Actor2D>(Act);
            
            //Get The Textures Array Ready.. I Mean We Know There IS Only One Texture In Actor2D.. So Get That Ready.. 
            std::vector<std::shared_ptr<Ermine::Texture>> Textures;
            Textures.emplace_back(PActor2D->GetSprite()->GetTexture());

            //Create And Return A std::unique_ptr Of The Renderable Texture Module..
            std::unique_ptr<RenderableTextureModule> Module(new RenderableTextureModule(PActor2D,Textures));

            std::shared_ptr<void> st = std::make_shared<void*>();
            Ermine::BroadcastComponent::BroadcastEvent(std::make_unique<Ermine::OnBeginEvent>(st));

            return Module;
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
