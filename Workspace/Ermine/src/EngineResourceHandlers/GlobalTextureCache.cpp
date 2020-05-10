#include "stdafx.h"
#include "GlobalTextureCache.h"

#pragma region DefineStaticVariables

Ermine::GlobalTextureCache* Ermine::GlobalTextureCache::TextureCache;

std::once_flag Ermine::GlobalTextureCache::InitializationFlag;

#pragma endregion DefineStaticVariables

namespace Ermine
{
	GlobalTextureCache::GlobalTextureCache()
	{
		//The Actual Files Of Copying The TextureFiles Over Is Handled By The AddNew TextureWindow When The Texture Gets Loaded
		//Into Memory..

#ifdef ER_DEBUG_DEVELOP

		std::filesystem::recursive_directory_iterator rdi("Texture");

		for (auto i : rdi)
		{
			Ermine::Texture* Tex = new Ermine::Texture(i,i.path().u8string());
			InternalBuffer.emplace(i.path(),Tex);
		}

#endif

	}

	GlobalTextureCache::~GlobalTextureCache()
	{
		for (auto& i : InternalBuffer)
		{
			delete i.second; //As The Global Texture Cache Owns All Its Textures It Alone Is Responsible For The Deletion of These Textures..
		}
	}

	GlobalTextureCache* GlobalTextureCache::Get()
	{
		std::call_once(InitializationFlag, []() {
			TextureCache = new GlobalTextureCache();
		});

		return TextureCache;
	}


	Texture* GlobalTextureCache::GetTextureFromFile(std::filesystem::path TextureFilePath)
	{
		auto FoundIter = InternalBuffer.find(TextureFilePath);
		if (FoundIter == InternalBuffer.end())
		{
			//This Means The Texture Was Not Found Add It into The Map..
			Texture* tex = new Texture(TextureFilePath);
			InternalBuffer[TextureFilePath] = tex;
			return tex;
		}
		else
		{
			return FoundIter->second;
		}
	}

	void GlobalTextureCache::PushTextureIntoCache(std::unique_ptr<Texture> tex)
	{
		auto FoundIter = InternalBuffer.find(tex->GetFilePath());
		if (FoundIter == InternalBuffer.end())
		{
			//This Means The Texture Was Not Found Really Add It into The Map..
			InternalBuffer[tex->GetFilePath()] = tex.release();
		}	
	}
	void GlobalTextureCache::ClearCache()
	{
		for (auto& i : InternalBuffer)
			delete i.second; //As The Global Texture Cache Owns All Its Textures It Alone Is Responsible For The Deletion of These Textures..
		
		InternalBuffer.clear();
	}
}