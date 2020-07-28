#include "stdafx.h"
#include "GlobalFontCache.h"

namespace Ermine
{
	//Start Static Definition Space//
	std::once_flag Ermine::GlobalFontCache::InitializationFlag;
	Ermine::GlobalFontCache* Ermine::GlobalFontCache::FontCachePointer = nullptr;
	bool Ermine::GlobalFontCache::GlobalFontCacheAlive = false;
	//Ended Static Definition Space//

	GlobalFontCache::GlobalFontCache()
	{}


	GlobalFontCache* GlobalFontCache::Get()
	{
		std::call_once(GlobalFontCache::InitializationFlag, []() {
			FontCachePointer = new GlobalFontCache();
			GlobalFontCacheAlive = true;
		});

		if(GlobalFontCacheAlive == true)
			return FontCachePointer;
		else
		{
			STDOUTDefaultLog_Critical("The Global Font Cache Does Not Exist And As Such We Cannot Give You A Line To The Cache ");
			return nullptr;
		}
	}

	void GlobalFontCache::DestroyGlobalFontCache()
	{
		GlobalFontCacheAlive = false;
		delete FontCachePointer;
	}


	void GlobalFontCache::LoadFontFromFile(std::filesystem::path FontFilePath)
	{
		
		auto Casket = FontsCache.find(FontSize);
		if (Casket != FontsCache.end())
		{
			auto MainCasket = Casket->second.find(HelperFilePathToFileName(FontFilePath));
			if (MainCasket != Casket->second.end())
			{
				//This Font Already Exists So Just Return To The User
				return; 
			}
		}
		Ermine::Font font(HelperFilePathToFileName(FontFilePath), FontFilePath, FontSize);
	
		std::unordered_map<std::string, Ermine::Font>& Cache = FontsCache[FontSize];
		Cache.emplace(HelperFilePathToFileName(FontFilePath), font);
	}


	void GlobalFontCache::SetFontSize(int NewFontSize)
	{
		FontSize = NewFontSize;
	}
	int GlobalFontCache::GetFontSize()
	{
		return FontSize;
	}


	Font const* GlobalFontCache::operator[](std::string FontName)
	{
		auto Casket = FontsCache.find(FontSize);
		if (Casket != FontsCache.end())
		{
			auto MainCasket = Casket->second.find(FontName);
			if (MainCasket != Casket->second.end())
			{
				//This Font Already Exists So Just Return To The User
				return &MainCasket->second;
			}
			else
			{
				STDOUTDefaultLog_Error("Requested A Font From The Global Font Cache That Does Not Exist");
				return nullptr;
			}
		}
		else
		{
			STDOUTDefaultLog_Error("Requested A Font From The Global Font Cache That Does Not Exist (Even A Single Font In This FontSize Also Does Not Exist : {%d})",FontSize);
			return nullptr;
		}
	}


	std::string GlobalFontCache::HelperFilePathToFileName(std::filesystem::path FilePath)
	{
		std::string FilePathString = FilePath.u8string();
		std::string FinalString;
		for (int i = 0; FilePathString.length(); i++)
		{
			if (FilePathString[i] == '.')
				break;

			if (FilePathString[i] == '/' || FilePathString[i] == '\\')
				FinalString.clear();

			FinalString.push_back(FilePathString[i]);
		}

		return FinalString;
	}
}
