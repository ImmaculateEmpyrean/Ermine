#pragma once
#include<iostream>
#include<vector>
#include<unordered_map>
#include<string>

#include<mutex>

#include<optional>

#include<algorithm>
#include<memory>

#include<filesystem>

#include <ft2build.h>
#include FT_FREETYPE_H
#include "freetype/freetype.h"
#include "freetype-gl-master/freetype-gl.h"
#include "FontRenderingSystem/Font.h"

namespace Ermine
{
	class GlobalFontCache
	{
	private:

		//Its Just Not Thats It.. You Wont Be Using It Anyway as it is singleton and all..
		GlobalFontCache(); 

	public:
		static GlobalFontCache* Get();
		static void DestroyGlobalFontCache();

		void LoadFontFromFile(std::filesystem::path FontFilePath);

		void SetFontSize(int NewFontSize);
		int  GetFontSize();

		//Returns Pointer To Constant Font.. The Font Size Being The Currently Set FontSize VAriable Inside The GlobalFontCache Variable..
		Font const* operator[](std::string FontName);
		
	public:

	protected:

	protected:

	private:
		std::string HelperFilePathToFileName(std::filesystem::path FilePath);

	private:
		static std::once_flag InitializationFlag;
		static GlobalFontCache* FontCachePointer;
		static bool GlobalFontCacheAlive;

		int FontSize = 20;

		/*This Is The Main Cache The Holds The Font Files To Be Used By The Engine At Will.. 
		  Int is The Font Size While std::string is The Font Name.. */
		std::unordered_map<int, std::unordered_map<std::string,Ermine::Font>> FontsCache;

	};
}