#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<utility>

#include "Graphics/Renderer/MaterialSystem/Texture.h"
#include "glm.hpp"

/*
	A Sprite Consists Of The Texture From Which its Drawn And The UV's Corresponing To The Texture
	If The Whole Texture is to be drawn set the uv's to include 0,0 and 1,1

	The Sprite As Of Now Cannot Be Drawn By The Renderer2D
*/

namespace Ermine
{
	class Sprite 
	{
	public:
		//Sprite Can Be Empty If It Needs To Be..
		Sprite() = default; 

		Sprite(std::shared_ptr<Texture> Tex, glm::vec2 BottomLeftUV,glm::vec2 TopRightUV);

		virtual ~Sprite();

	public:
		virtual std::shared_ptr<Texture> GetTexture();

		virtual glm::vec2 GetBottomLeftUV();
		virtual glm::vec2 GetTopRightUV();

		void SetTexture(std::shared_ptr<Texture> tex);

		void SetBottomLeftUV(glm::vec2 UV);
		void SetTopRightUV(glm::vec2 UV);

		//Note- this operator is not to be used anywhere except the renderer and there also to check if two sprites have the same texture Nothing else..
		bool operator ==(Sprite rhs); 

		//This Function Is To Be USed To Compare Sprites..
		bool Equals(Sprite rhs);

	public:

	protected:
		//This For Now Is Primarily Used By Actor2D To Construct A Renderable2D..
		//Sprite(std::shared_ptr<Texture> Tex, glm::vec2 BottomLeftUV, glm::vec2 TopRightUV, Ermine::VertexArray Vao, Ermine::Material Mat); 

	protected:

	private:

	private:
		std::shared_ptr<Texture> TextureContainingTheSprite; //This Texture Is Neither Created Or Owned By The Sprite..
		
		glm::vec2 BottomLeftUV = {0.0f,0.0f};
		glm::vec2 TopRightUV = {1.0f,1.0f};

	};
}