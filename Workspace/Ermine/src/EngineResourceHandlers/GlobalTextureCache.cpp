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

		std::filesystem::recursive_directory_iterator rdi("Texture");

		for (auto i : rdi)
		{
			Ermine::Texture* Tex = new Ermine::Texture(i);
			InternalBuffer.emplace_back(Tex);
		}
	}

	GlobalTextureCache::~GlobalTextureCache()
	{
		for (auto i : InternalBuffer)
		{
			delete i; //As The Global Texture Cache Owns All Its Textures It Alone Is Responsible For The Deletion of These Textures..
		}
	}

	GlobalTextureCache* GlobalTextureCache::Get()
	{
		std::call_once(InitializationFlag, []() {
			TextureCache = new GlobalTextureCache();
		});

		return TextureCache;
	}

	std::vector<Texture*>& GlobalTextureCache::GetCache()
	{
		return InternalBuffer;
	}
	void GlobalTextureCache::PushTextureIntoCache(Texture* tex)
	{
		InternalBuffer.emplace_back(tex);
	}
}