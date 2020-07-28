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

		//All Inclusive Way To Create A Font 
		Font(std::string FontName, std::filesystem::path FontFilePath,int FontSize);

		//Destructor..
		~Font();

	public:
		Font(const Font& rhs);
		Font(Font&& rhs);

		Font operator=(const Font& rhs);
		Font operator=(Font&& rhs);

	public:

	protected:

	protected:

	private:
		void HelperLoadTextureAtlasAndFont(std::filesystem::path FontFilePath, int FontSize);

		void HelperCopyConstructor(const Font& rhs);
		void HelperMoveConstructor(Font&& rhs);

	public: //For Now Change It To Private Once Its Done
		//The Default Name Is Populated Using The FileName If The Constructor Taking The FontName Is Not USed..
		std::string FontName;
		int FontSize = 0;
		std::filesystem::path FontFilePath;

		ftgl::texture_font_t* FontTexture = nullptr;
		ftgl::texture_atlas_t* FontAtlas = nullptr;
	};
}