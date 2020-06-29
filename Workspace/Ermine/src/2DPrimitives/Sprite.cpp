#include "stdafx.h"
#include "Sprite.h"

namespace Ermine
{
	Sprite::Sprite(std::shared_ptr<Texture> Tex, glm::vec2 BottomLeftUV, glm::vec2 TopRightUV)
		:
		TextureContainingTheSprite(Tex),
		BottomLeftUV(BottomLeftUV),
		TopRightUV(TopRightUV)
	{}


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
