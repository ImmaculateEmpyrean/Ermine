#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<unordered_map>

#include<mutex>

#include<fstream>
#include<filesystem>
#include<memory>

#include<optional>

#include "Graphics/Renderer/MaterialSystem/Texture.h"

namespace std {
	template <>
	struct hash<filesystem::path> {
		std::size_t operator()(const filesystem::path& path) const {
			return hash_value(path);
		}
	};
}

namespace Ermine
{
	class TextureViewWizard; //Forward Declaration To Make Friend..

	class GlobalTextureCache
	{
	private:
		GlobalTextureCache(); //This Class Is Being Designed as a Singleton Hence The Private Constructor..
		~GlobalTextureCache();

	public:
		static GlobalTextureCache* Get();

		//This Function Loads A Texture From File If It Does Not Already Exist Will Return A Texture Pointer To The Memory 
		//This Loaded Is Owned By The Cache And Will Delete Ity only When The Clear Instruction Is Given..
		Texture* GetTextureFromFile(std::filesystem::path TextureFilePath);

		//This Push Is A Very Optimized Function It Does Not Simply Push in Redundant Instead If Something Is Repeated It Does Not Push..
		void PushTextureIntoCache(std::unique_ptr<Texture> tex);

		void ClearCache(); //This Invalidates All Previously Held Tokens..

	public:

	protected:

	protected:

	private:

	private:
		static std::once_flag InitializationFlag;

		static GlobalTextureCache* TextureCache;

		std::unordered_map<std::filesystem::path,Texture*,std::hash<std::filesystem::path>> InternalBuffer;

		friend class TextureViewWizard;
	};
}