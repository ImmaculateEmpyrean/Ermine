#include "stdafx.h"
#include "Font.h"

Ermine::Font::Font(std::filesystem::path FontFilePath,int FontSize)
	:
	FontFilePath(FontFilePath),
	FontSize(FontSize)
{
	HelperLoadTextureAtlasAndFont(FontFilePath, FontSize);
}

Ermine::Font::Font(std::string FontName, std::filesystem::path FontPathInFile)
	:
	FontName(FontName),
	FontFilePath(FontFilePath)
{
	HelperLoadTextureAtlasAndFont(FontPathInFile, 20);
}

Ermine::Font::Font(std::string FontName, std::filesystem::path FontFilePath, int FontSize)
	:
	FontName(FontName),
	FontSize(FontSize),
	FontFilePath(FontFilePath)
{
	HelperLoadTextureAtlasAndFont(FontFilePath, FontSize);
}

Ermine::Font::~Font()
{
	if (FontTexture == nullptr)
	{
		ftgl::texture_font_delete(FontTexture);
		FontTexture = nullptr;
	}

	if (FontAtlas == nullptr)
	{
		ftgl::texture_atlas_delete(FontAtlas);
		FontAtlas = nullptr;
	}
}


Ermine::Font::Font(const Font& rhs)
{
	HelperCopyConstructor(rhs);
}
Ermine::Font Ermine::Font::operator=(const Font& rhs)
{
	HelperCopyConstructor(rhs);
	return *this;
}

Ermine::Font::Font(Font&& rhs)
{
	HelperMoveConstructor(std::move(rhs));
}
Ermine::Font Ermine::Font::operator=(Font&& rhs)
{
	HelperMoveConstructor(std::move(rhs));
	return *this;
}


void Ermine::Font::HelperLoadTextureAtlasAndFont(std::filesystem::path FontFilePath, int FontSize)
{
	//Create A Font Atlas To Be Used By The Font Texture
	FontAtlas = ftgl::texture_atlas_new(512, 512,1);

	//Give The Font Atlas To A Structure Which Is Much More Suitable For Getting Us The Glyphs We Want..
	FontTexture = ftgl::texture_font_new_from_file(FontAtlas, FontSize, FontFilePath.u8string().c_str());

	//This for Loop Gets And Loads All Relevant Characters Inside The Font Atlas So That Subsequent Calls May Be Faster..
	for (int i = 32; i < 127; i++)
		ftgl::texture_font_get_glyph(FontTexture, (char)i);
}


void Ermine::Font::HelperCopyConstructor(const Font& rhs)
{
	FontName	 = rhs.FontName;
	FontSize	 = rhs.FontSize;
	FontFilePath = rhs.FontFilePath;

	HelperLoadTextureAtlasAndFont(FontFilePath, FontSize);
}

void Ermine::Font::HelperMoveConstructor(Font&& rhs)
{
	FontName	 = std::move(rhs.FontName);
	FontSize     = std::move(rhs.FontSize);
	FontFilePath = std::move(rhs.FontFilePath);

	FontTexture = rhs.FontTexture;
	rhs.FontTexture = nullptr;

	FontAtlas = rhs.FontAtlas;
	rhs.FontAtlas = nullptr;
}
