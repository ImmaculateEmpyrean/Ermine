#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<utility>

#include "Graphics/Renderer/MaterialSystem/Texture.h"
#include "glm.hpp"

/*
	A Sprite Is A Singular Quad Entity Which Can Be Drawn By The 2D Renderer.
	A Sprite Consists Of The Texture From Which its Drawn And The UV's Corresponing To The Texture
	If The Whole Texture is to be drawn set the uv's to include 0,0 and 1,1
*/

namespace Ermine
{
	class Sprite
	{
	public:
		Sprite() = delete; //There is no reason for an empty sprite donot have one..
		Sprite(Texture* Tex, glm::vec2 BottomLeftUV,glm::vec2 TopRightUV); 

	public:
		Texture* GetTexture();

		glm::vec2 GetBottomLeftUV();
		glm::vec2 GetTopRightUV();

		void SetTexture(Texture* tex);

		void SetBottomLeftUV(glm::vec2 UV);
		void SetTopRightUV(glm::vec2 UV);

		//Note- this operator is not to be used anywhere except the renderer and there also to check if two sprites have the same texture Nothing else..
		bool operator ==(Sprite rhs); 

	public:

	protected:

	protected:

	private:

	private:
		Texture* TextureContainingTheSprite; //This Texture Is Neither Created Or Owned By The Sprite..
		
		glm::vec2 BottomLeftUV;
		glm::vec2 TopRightUV;

	};
}