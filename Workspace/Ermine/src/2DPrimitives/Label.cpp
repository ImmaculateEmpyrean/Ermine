#include "stdafx.h"
#include "Label.h"

#include "EngineResourceHandlers/GlobalTextureCache.h"

Ermine::Label::Label(std::string Text, glm::vec3 Color)
	:
	Text(Text),
	TextColor(Color),
	MovableObject(glm::mat4(0.0f)),
	RenderableTextureModule()
{
	auto FontManager = Ermine::GlobalFontCache::Get();
	fnt = FontManager->GetDefaultFont();

	//This Call Will Fix All The Other Stuff..
	HelperConstructLabel();
}

Ermine::Label::Label(std::string Text, glm::mat4 ModelMatrix, glm::vec3 Color)
	:
	Text(Text),
	TextColor(Color),
	MovableObject(ModelMatrix),
	RenderableTextureModule()
{
	auto FontManager = Ermine::GlobalFontCache::Get();
	fnt = FontManager->GetDefaultFont();

	//This Call Will Fix All The Other Stuff..
	HelperConstructLabel();
}

Ermine::Label::Label(std::string Text, glm::vec3 Color, std::shared_ptr<Font> fnt, glm::mat4 ModelMatrix)
	:
	Text(Text),
	TextColor(Color),
	fnt(fnt),
	MovableObject(ModelMatrix),
	RenderableTextureModule()
{
	HelperConstructLabel();
}

Ermine::Label::Label(std::string Text, glm::vec3 Color, std::filesystem::path FontFilePath ,int FontSize, glm::mat4 ModelMatrix)
	:
	Text(Text),
	TextColor(Color),
	MovableObject(ModelMatrix),
	RenderableTextureModule()
{
	auto FontManager = Ermine::GlobalFontCache::Get();
	FontManager->LoadFontFromFile(FontFilePath);
	FontManager->SetFontSize(FontSize);
	this->fnt = FontManager->GetFontFromFilePath(FontFilePath);

	HelperConstructLabel();
}

Ermine::Label::Label(std::string Text, glm::vec3 Color, std::string FontName,int FontSize, glm::mat4 ModelMatrix)
	:
	Text(Text),
	TextColor(Color),
	MovableObject(ModelMatrix),
	RenderableTextureModule()
{
	auto FontManager = Ermine::GlobalFontCache::Get();
	FontManager->SetFontSize(FontSize);
	this->fnt = (*FontManager)[FontName];

	HelperConstructLabel();
}

void Ermine::Label::ClearText()
{
	Text.clear();
}

void Ermine::Label::SetTextColor(glm::vec3 TextColor)
{
	this->TextColor = TextColor;
}

void Ermine::Label::ClearAndWriteText(std::string Text)
{
	this->Text = Text;
}

void Ermine::Label::AppendTextToBack(std::string Text)
{
	this->Text = this->Text + Text;
}

void Ermine::Label::AppendTextToBegin(std::string Text)
{
	this->Text = Text + this->Text;
}

void Ermine::Label::HelperConstructLabel()
{
	//This Function Is Pretty Big And Important And It Is Better We Take A Full Session To Implement It.. And Check It A Bit..
	std::vector<float> VertexBuffer;
	std::vector<uint32_t> IndexBuffer;

	//Pseudo GLobal Variables Useful For The For Loop Ahead..
	float PositionXofGlyph = 0.0f;
	float PositionYofGlyph = 0.0f;

	int NumberOfVertices = 0;

	for (char ch : Text)
	{
		auto CharacterGlyph = fnt->GetCharGlyph(ch);
		
		//Start Variable Declaration Field
		VertexTextured TopLeft;
		VertexTextured TopRight;

		VertexTextured BottomLeft;
		VertexTextured BottomRight;

		//Start All Of These Are In Poxel Coordinates
		float PositionXMin = PositionXofGlyph;
		float PositionXMax = PositionXMin + CharacterGlyph->width;   // Ermine::NormalizationFunction(, 0.0f, 1.0f, 0.0f, Ermine::GetScreenWidth());

		float PositionYMin = PositionYofGlyph;
		float PositionYMax = PositionYMin + CharacterGlyph->height;
		//Ended All Of These Are In Poxel Coordinates

		//Start Construction OF The Vertices..

		//s stands for x s0 for top left s1 for bottom right meaning s0 = left s1 = right
		//t stands for y t0 for top left t1 for bottom right meaning t0 = top t1 = bottom
		
		//Start Construction Of Top Left Vertex..
		TopLeft.SetPositonCoordinates({PositionXMin,PositionYMin,0.0f});
		TopLeft.SetVertexColorValue(TextColor);
		TopLeft.SetVertexUV({CharacterGlyph->s0,CharacterGlyph->t0});
		TopLeft.SetTextureNumber({ 0.0f });
		//Ended Construction Of Top Left Vertex..
				
		//Start Construction Of Bottom Left Vertex..
		BottomLeft.SetPositonCoordinates({ PositionXMin,PositionYMax,0.0f });
		BottomLeft.SetVertexColorValue(TextColor);
		BottomLeft.SetVertexUV({ CharacterGlyph->s0,CharacterGlyph->t1 });
		BottomLeft.SetTextureNumber({ 0.0f });
		//Ended Construction Of Bottom Left Vertex..

		//Start Construction Of Top Right Vertex..
		TopRight.SetPositonCoordinates({ PositionXMax,PositionYMin,0.0f });
		TopRight.SetVertexColorValue(TextColor);
		TopRight.SetVertexUV({ CharacterGlyph->s1,CharacterGlyph->t0 });
		TopRight.SetTextureNumber({ 0.0f });
		//Ended Construction Of Top Right Vertex..

		//Start Construction Of Bottom Right Vertex..
		BottomRight.SetPositonCoordinates({ PositionXMax,PositionYMax,0.0f });
		BottomRight.SetVertexColorValue(TextColor);
		BottomRight.SetVertexUV({ CharacterGlyph->s1,CharacterGlyph->t1 });
		BottomRight.SetTextureNumber({ 0.0f });
		//Ended Construction Of Bottom Right Vertex..

		//Ended Construction Of The Vertices..

		PositionXofGlyph = PositionXofGlyph + CharacterGlyph->advance_x;//Ermine::NormalizationFunction(CharacterGlyph->advance_x, 0.0f, 1.0f, 0.0f, (float)Ermine::GetScreenWidth());
		NumberOfVertices = NumberOfVertices + 4;

		//Counter Clockwise
		VertexBuffer = VertexBuffer + TopRight;
		VertexBuffer = VertexBuffer + BottomRight;
		VertexBuffer = VertexBuffer + BottomLeft;
		VertexBuffer = VertexBuffer + TopLeft;
	}

	IndexBuffer = VertexTextured::GenerateIndexBufferQuad(NumberOfVertices);

	VertexArray Vao(VertexBuffer, IndexBuffer);
	static std::vector<VertexAttribPointerSpecification> Spec = {
				{3,GL_FLOAT,false},
				{3,GL_FLOAT,false},
				{2,GL_FLOAT,false},
				{1,GL_FLOAT,false}
	};

	Vao.SetVertexAttribArray(Spec);

	RenderableTextureModule::SetVertexArray(std::move(Vao));
	RenderableTextureModule::SetMaterial(Material(std::filesystem::path("Shader/Vertex/TextRenderingVertexShader.vert"),std::filesystem::path("Shader/Fragment/TextRenderingFragmentShader.frag")));
	//RenderableTextureModule::SetMaterial(Material("Shader/TextBaseMaterial.json"));

	

}

std::vector<int> Ermine::Label::BindTexturesContained()
{
	auto TextureCache = Ermine::GlobalTextureCache::Get();
	
	std::vector<int> Array;
	Array.resize(16, 0);
	Array[0] = TextureCache->Bind(fnt->FontAtlas->id);

	return Array;
}
