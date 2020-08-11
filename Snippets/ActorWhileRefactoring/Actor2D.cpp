#include "stdafx.h"
#include "Actor2D.h"

#include "glm.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Graphics/Renderer/RendererPrimitives/VertexArray.h"

#include "2DPrimitives/Constructs/VertexBase.h"
#include "2DPrimitives/Constructs/VertexTextured.h"

namespace Ermine
{
	Actor2D::Actor2D(std::shared_ptr<Sprite> Spr)
		:
		Actor2DBase(Spr)
	{}
	Actor2D::Actor2D(std::vector<std::shared_ptr<Sprite>> SpriteBuffer)
		:
		Actor2DBase(SpriteBuffer)
	{}

	Actor2D::Actor2D(std::shared_ptr<Sprite> Spr, glm::mat4 ModelMatrix)
		:
		MovableObject(ModelMatrix),
		Actor2DBase(Spr)
	{}

	Actor2D::Actor2D(std::vector<std::shared_ptr<Sprite>> SpriteBuffer, glm::mat4 ModelMatrix)
		:
		MovableObject(ModelMatrix),
		Actor2DBase(SpriteBuffer)
	{}

	Actor2D::~Actor2D()
	{
		//This Destructor Is Not Needed In This Level But For The Sake Of Overriding Down The Line It Is Still Kept Here..
	}


	glm::mat4 Actor2D::GetModelMatrix()
	{
		return MovableObject::RecievedModelMatrix * MovableObject::TranslationMatrix;
	}

	std::vector<float> Actor2D::GetModelSpaceCoordinates()
	{
		Ermine::VertexTextured TopRight(Quad::GetModelCoordinatesTopRight());
		Ermine::VertexTextured BottomRight(Quad::GetModelCoordinatesBottomRight());
		Ermine::VertexTextured BottomLeft(Quad::GetModelCoordinatesBottomLeft());
		Ermine::VertexTextured TopLeft(Quad::GetModelCoordinatesTopLeft());

		glm::vec3 TopRightPos = TopRight.GetPositionCoordinates();
		glm::vec4 TopRightPos4 = glm::vec4(TopRightPos, 0.0f);

		glm::vec3 BottomRightPos = BottomRight.GetPositionCoordinates();
		glm::vec4 BottomRightPos4 = glm::vec4(BottomRightPos, 0.0f);

		glm::vec3 BottomLeftPos = BottomLeft.GetPositionCoordinates();
		glm::vec4 BottomLeftPos4 = glm::vec4(BottomLeftPos, 0.0f);

		glm::vec3 TopLeftPos = TopLeft.GetPositionCoordinates();
		glm::vec4 TopLeftPos4 = glm::vec4(TopLeftPos, 0.0f);

		TopRightPos4 = RotationMatrix * TopRightPos4;
		BottomRightPos4 = RotationMatrix * BottomRightPos4;
		BottomLeftPos4 = RotationMatrix * BottomLeftPos4;
		TopLeftPos4 = RotationMatrix * TopLeftPos4;

		TopRightPos4 = ScaleMatrix * TopRightPos4;
		BottomRightPos4 = ScaleMatrix * BottomRightPos4;
		BottomLeftPos4 = ScaleMatrix * BottomLeftPos4;
		TopLeftPos4 = ScaleMatrix * TopLeftPos4;

		TopRight.SetPositonCoordinates(TopRightPos4);
		BottomRight.SetPositonCoordinates(BottomRightPos4);
		BottomLeft.SetPositonCoordinates(BottomLeftPos4);
		TopLeft.SetPositonCoordinates(TopLeftPos4);

		TopRight.SetVertexUV(glm::vec2(Actorsprite->GetTopRightUV().x, Actorsprite->GetBottomLeftUV().y));
		BottomRight.SetVertexUV(glm::vec2(Actorsprite->GetTopRightUV().x, Actorsprite->GetTopRightUV().y));
		BottomLeft.SetVertexUV(glm::vec2(Actorsprite->GetBottomLeftUV().x, Actorsprite->GetTopRightUV().y));
		TopLeft.SetVertexUV(glm::vec2(Actorsprite->GetBottomLeftUV().x, Actorsprite->GetBottomLeftUV().y));

		std::vector<float> ModelCoordinates;
		ModelCoordinates = TopRight;
		ModelCoordinates = ModelCoordinates + BottomRight;
		ModelCoordinates = ModelCoordinates + BottomLeft;
		ModelCoordinates = ModelCoordinates + TopLeft;
		
		return ModelCoordinates;
	}
}
