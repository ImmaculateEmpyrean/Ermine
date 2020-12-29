#pragma once
#include<iostream>
#include<vector>
#include<string>

#include "2DPrimitives/Sprite.h"

namespace Ermine
{
	struct SpriteRenderComponent
	{
		SpriteRenderComponent()
			:
			Spr(Ermine::Sprite::GetNullSprite())
		{}

		SpriteRenderComponent(std::shared_ptr<Ermine::Sprite> Spr)
			:
			Spr(Spr)
		{}

	public:
		std::shared_ptr<Ermine::Sprite> Spr;
	};
}