#include "stdafx.h"
#include "Sprite.h"

#include "EngineResourceHandlers/GlobalTextureCache.h"

namespace Ermine
{
	Sprite::Sprite(std::shared_ptr<Texture> Tex, glm::vec2 BottomLeftUV, glm::vec2 TopRightUV)
		:
		TextureContainingTheSprite(Tex),
		BottomLeftUV(BottomLeftUV),
		TopRightUV(TopRightUV)
	{}
	Sprite::~Sprite()
	{}

	std::shared_ptr<Ermine::Sprite> Sprite::GenerateSprite(std::filesystem::path TexturePath, glm::vec2 BottomLeftUV, glm::vec2 TopRightUV)
	{
		auto Cache = Ermine::GlobalTextureCache::Get();
		std::shared_ptr<Ermine::Texture> Text = Cache->GetTextureFromFile(TexturePath);

		std::shared_ptr<Ermine::Sprite> Spr = std::make_shared<Ermine::Sprite>(Text,BottomLeftUV,TopRightUV);
		return Spr;
	}

	std::shared_ptr<Ermine::Sprite> Sprite::GenerateSprite(std::shared_ptr<Texture> Tex, glm::vec2 BottomLeftUV, glm::vec2 TopRightUV)
	{
		std::shared_ptr<Ermine::Sprite> Spr = std::make_shared<Ermine::Sprite>(Tex, BottomLeftUV, TopRightUV);
		return Spr;
	}

	std::shared_ptr<Ermine::Sprite> Sprite::GetNullSprite()
	{
		std::shared_ptr<Ermine::Sprite> Spr(new Ermine::Sprite(Ermine::Texture::GetNullTexture(), glm::vec2(0.0f, 0.0f),glm::vec2(1.0f,1.0f)));
		return Spr;
	}


	std::shared_ptr<Texture> Sprite::GetTexture()
	{
		return TextureContainingTheSprite;
	}

	glm::vec2 Sprite::GetBottomLeftUV()
	{
		return BottomLeftUV;
	}
	glm::vec2 Sprite::GetTopRightUV()
	{
		return TopRightUV;
	}


	void Sprite::SetTexture(std::shared_ptr<Texture> tex)
	{
		TextureContainingTheSprite = tex;
	}

	void Sprite::SetBottomLeftUV(glm::vec2 UV)
	{
		BottomLeftUV = UV;
	}
	void Sprite::SetTopRightUV(glm::vec2 UV)
	{
		TopRightUV = UV;
	}


	bool Sprite::operator==(Sprite rhs)
	{
		//If You Are Not in The Renderer And Using This Function, You Might Be Doing Something Wrong.. Please See What This Function is doing
		//It is not INTUITIVE... It does not even compare sprites..
		if (rhs.TextureContainingTheSprite == TextureContainingTheSprite)
			return true;
		else
			return false;
	}
	bool Sprite::Equals(Sprite rhs)
	{
		if (rhs.TextureContainingTheSprite == TextureContainingTheSprite)
		{
			if (BottomLeftUV == rhs.BottomLeftUV && TopRightUV == rhs.TopRightUV)
				return true;
			else return false;
		}
		else return false;
	}
}
