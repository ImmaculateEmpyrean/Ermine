#include "stdafx.h"
#include "Font.h"

Ermine::Font::Font(std::filesystem::path FontFilePath,int FontSize)
{
	HelperLoadTextureAtlasAndFont(FontFilePath, FontSize);
}

Ermine::Font::Font(std::string FontName, std::filesystem::path FontPathInFile)
{
	HelperLoadTextureAtlasAndFont(FontPathInFile, FontSize);
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

void Ermine::Font::HelperLoadTextureAtlasAndFont(std::filesystem::path FontFilePath, int FontSize)
{
	FontAtlas = ftgl::texture_atlas_new(512, 512,1);
	ftgl::texture_atlas_upload(FontAtlas);

	ftgl::ivec4 region = ftgl::texture_atlas_get_region(FontAtlas, 512, 512);

	//FontTexture = ftgl
	FontTexture = ftgl::texture_font_new_from_file(FontAtlas, 20, "SourceSansPro-Light.ttf");// FontFilePath.u8string().c_str());
	
	ftgl::texture_font_get_glyph(FontTexture, 'H');
	ftgl::texture_font_get_glyph(FontTexture, 'E');
	//ftgl::texture_font_get_glyph(FontTexture, 'L');
	//ftgl::texture_font_get_glyph(FontTexture, 'J');
	//ftgl::texture_font_get_glyph(FontTexture, 'O');

	
	//std::cout<<ftgl::texture_font_load_glyph(FontTexture, "E");

	//for (int i = 32; i < 127; i++);
	
	//std::cout << t->width;
	
}
