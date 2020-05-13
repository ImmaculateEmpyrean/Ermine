#include "stdafx.h"
#include "Actor2D.h"

#include "glm.hpp"

namespace Ermine
{
	Actor2D::Actor2D(Sprite* Spr)
		:
		Actorsprite(Spr)
	{
		ModelMatrix = glm::mat3();
	}
	Actor2D::Actor2D(Sprite* Spr, glm::mat3 ModelMatrix)
	{
	}

	Actor2D::~Actor2D()
	{
	}


	void Actor2D::Translate(int x, int y)
	{
		
	}
	void Actor2D::Translate(glm::vec2 TranslateByHowMuch)
	{
	}

	void Actor2D::Rotate(int Pi)
	{
	}
	void Actor2D::Rotate(float Degrees)
	{
	}

	void Actor2D::Scale(int x, int y)
	{
	}
	void Actor2D::Scale(glm::vec2 ScaleByHowMuch)
	{
	}
}
