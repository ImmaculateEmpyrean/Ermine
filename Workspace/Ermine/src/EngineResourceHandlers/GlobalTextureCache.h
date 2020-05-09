#pragma once
#include<iostream>
#include<vector>
#include<string>

#include<mutex>

#include "Graphics/Renderer/MaterialSystem/Texture.h"

namespace Ermine
{
	class GlobalTextureCache
	{
	private:
		GlobalTextureCache(); //This Class Is Being Designed as a Singleton Hence The Private Constructor..
		~GlobalTextureCache();

	public:
		static GlobalTextureCache* Get();

		std::vector<Texture*>& GetCache();
		int PushTextureIntoCache(Texture* tex);

		void ClearCache(); //This Invalidates All Previously Held Tokens..

	public:

	protected:

	protected:

	private:

	private:
		static std::once_flag InitializationFlag;

		static GlobalTextureCache* TextureCache;

		std::vector<Texture*> InternalBuffer;
	};
}