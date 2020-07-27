#pragma once
#include<iostream>
#include<vector>
#include<string>

#include<fstream>
#include<filesystem>

#include<memory>
#include<algorithm>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "freetype/freetype.h"
#include "freetype-gl-master/freetype-gl.h"

namespace Ermine
{
	class Font
	{
	public:
		//An Empty Font Can Exist But This Is Dangerous If It Is Inside GlobalFontCache We Would HAve No Way Of Accessing It..
		Font() = default;

		//This Much More Of A Default Way Of Constructing A Font..
		Font(std::filesystem::path FontPathInFile,int FontSize);

		//This I Guess Is A Specialized Way To Create A Font 
		Font(std::string FontName,std::filesystem::path FontFilePath);

		//Destructor..
		~Font();

	public:

	public:

	protected:

	protected:

	private:
		void HelperLoadTextureAtlasAndFont(std::filesystem::path FontFilePath, int FontSize);

	public: //For Now Change It Ti Private Once Its Done
		//The Default Name Is Populated Using The FileName If The Constructor Taking The FontName Is Not USed..
		std::string FontName;

		//The Size Must Be Specified When Loading In The Font. 0 Means No Font Is Loaded..
		int FontSize = 0;

		ftgl::texture_font_t* FontTexture = nullptr;
		ftgl::texture_atlas_t* FontAtlas = nullptr;
	};
}