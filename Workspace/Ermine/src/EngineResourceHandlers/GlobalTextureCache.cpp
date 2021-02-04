#include "stdafx.h"
#include "GlobalTextureCache.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

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
			std::shared_ptr<Texture> Tex = std::make_shared<Texture>(i, i.path().u8string());
			InternalBuffer.emplace(i.path(),Tex);
		}

#endif
	}

	GlobalTextureCache::~GlobalTextureCache()
	{
		for (auto& i : InternalBuffer)
		{
			i.second.reset(); //As The Global Texture Cache Owns All Its Textures It Alone Is Responsible For The Deletion of These Textures..
		}
		InternalBuffer.clear();
	}

	GlobalTextureCache* GlobalTextureCache::Get()
	{
		std::call_once(InitializationFlag, []() {
			TextureCache = new GlobalTextureCache();
		});

		return TextureCache;
	}

	void GlobalTextureCache::ShutDownGlobalTextureCache()
	{
		delete TextureCache; //Destroy The TextureCache Global Object To Shutdown The System..
	}


	std::shared_ptr<Texture> GlobalTextureCache::GetTextureFromFile(std::filesystem::path TextureFilePath)
	{
		auto FoundIter = InternalBuffer.find(TextureFilePath);
		if (FoundIter == InternalBuffer.end())
		{
			//This Means The Texture Was Not Found Add It into The Map..
			std::shared_ptr<Texture> tex(new Texture(TextureFilePath));
			InternalBuffer[TextureFilePath] = tex;
			return tex;
		}
		else
		{
			return FoundIter->second;
		}
	}

	std::vector<std::shared_ptr<Texture>> GlobalTextureCache::GetAllTexturesInCache()
	{
		std::vector<std::shared_ptr<Texture>> Vec;

		for (auto i : InternalBuffer)
			Vec.emplace_back(i.second);

		return Vec;
	}

	void GlobalTextureCache::PushTextureIntoCache(std::unique_ptr<Texture> tex)
	{
		auto FoundIter = InternalBuffer.find(tex->GetFilePath());
		if (FoundIter == InternalBuffer.end())
		{
			//This Means The Texture Was Not Found Really Add It into The Map..
			//Texture* t = tex.release();
			InternalBuffer[tex->GetFilePath()].reset(tex.release());// = std::make_shared<Texture>();
		}	
	}
	void GlobalTextureCache::ClearCache()
	{
		for (auto& i : InternalBuffer)
			i.second.reset();
		
		InternalBuffer.clear();
	}
	/*int GlobalTextureCache::Bind(std::shared_ptr<Texture> Tex)
	{
		BindCounter = BindCounter + 1;
		if (BindCounter == 17)
			BindCounter = 1;

		Tex->Bind(BindCounter);

		//BindCounter++;

		return BindCounter;// -1;
	}
	int GlobalTextureCache::Bind(unsigned int OpenGlTexture)
	{
		BindCounter = BindCounter + 1;

		if (BindCounter == 17)
			BindCounter = 0;

		glActiveTexture(GL_TEXTURE0 + BindCounter); // activate the texture unit first before binding texture
		glBindTexture(GL_TEXTURE_2D, OpenGlTexture);

		return BindCounter;
	}*/
}