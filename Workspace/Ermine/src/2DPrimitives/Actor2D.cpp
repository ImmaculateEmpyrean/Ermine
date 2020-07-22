#include "stdafx.h"
#include "Actor2D.h"

#include "glm.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Ermine
{
	Actor2D::Actor2D(std::shared_ptr<Sprite> Spr)
		:
		Actorsprite(Spr)
	{
		TranslationMatrix = glm::mat4(1.0f);
		RotationMatrix = glm::mat4(1.0f);
		ScaleMatrix = glm::mat4(1.0f);

		RecievedModelMatrix = glm::mat4(1.0f);
		
		HelperInitializeRenderable2D();
	}
	Actor2D::Actor2D(std::shared_ptr<Sprite> Spr, glm::mat4 ModelMatrix)
		:
		Actorsprite(Spr),
		RecievedModelMatrix(ModelMatrix)
	{
		TranslationMatrix = glm::mat4(1.0f);
		RotationMatrix = glm::mat4(1.0f);
		ScaleMatrix = glm::mat4(1.0f);

		HelperInitializeRenderable2D();
	}

	Actor2D::~Actor2D()
	{}


	void Actor2D::Translate(float x, float y)
	{
		TranslationMatrix = glm::translate(TranslationMatrix, { x,y,0.0f });
	}
	void Actor2D::Translate(glm::vec2 TranslateByHowMuch)
	{
		TranslationMatrix = glm::translate(TranslationMatrix, { TranslateByHowMuch.x,TranslateByHowMuch.y,0.0f });
	}
	void Actor2D::ClearTranslations()
	{
		TranslationMatrix = glm::mat4(1.0f);
	}

	void Actor2D::Rotate(float Angle, bool IsInPI)
	{
		if(!IsInPI)
			RotationMatrix = glm::rotate(RotationMatrix, glm::radians(Angle), glm::vec3(0.0, 0.0, 1.0)); //Rotate On Z Axis Since This is 2D..
		else
			RotationMatrix = glm::rotate(RotationMatrix, Angle, glm::vec3(0.0, 0.0, 1.0)); //Rotate On Z Axis Since This is 2D..
	}
	void Actor2D::ClearRotations()
	{
		RotationMatrix = glm::mat4(1.0f);
	}

	void Actor2D::Scale(float x, float y)
	{
		ScaleMatrix = glm::scale(ScaleMatrix, { x,y,1.0f });
	}
	void Actor2D::Scale(glm::vec2 ScaleByHowMuch)
	{
		ScaleMatrix = glm::scale(ScaleMatrix, { ScaleByHowMuch.x,ScaleByHowMuch.y,1.0f });
	}
	void Actor2D::ClearScale()
	{
		ScaleMatrix = glm::mat4(1.0f);
	}


	std::vector<float> Actor2D::GetModelSpaceCoordinates()
	{
		std::vector<float> ModelCoordinates = Quad::GetModelCoordinates();

		//Top Right..
		ModelCoordinates[6] = Actorsprite->GetTopRightUV().x;
		ModelCoordinates[7] = Actorsprite->GetTopRightUV().y;

		//Bottom Right
		ModelCoordinates[14] = Actorsprite->GetTopRightUV().x;//ModelCoordinates[14] = Actorsprite->GetBottomLeftUV().x;
		ModelCoordinates[15] = Actorsprite->GetBottomLeftUV().y;

		//Bottom Left
		ModelCoordinates[22] = Actorsprite->GetBottomLeftUV().x;
		ModelCoordinates[23] = Actorsprite->GetBottomLeftUV().y;

		//Top Left
		ModelCoordinates[30] = Actorsprite->GetBottomLeftUV().x;
		ModelCoordinates[31] = Actorsprite->GetTopRightUV().y;

		return ModelCoordinates;
	}

	std::vector<uint32_t> Actor2D::GetModelSpaceIndices()
	{
		return Quad::GetModelIndices();
	}

	glm::mat4 Actor2D::GetModelMatrix()
	{
		return RecievedModelMatrix * ScaleMatrix * RotationMatrix * TranslationMatrix;
	}

	std::shared_ptr<Sprite> Actor2D::GetSprite()
	{
		return Actorsprite;
	}
	void Actor2D::SetSprite(std::shared_ptr<Sprite> Sprite)
	{
		Actorsprite = Sprite;
	}


	void Actor2D::HelperInitializeRenderable2D()
	{
		auto Vao = Ermine::VertexArray(VertexBuffer(GetModelSpaceCoordinates()), IndexBuffer(GetModelSpaceIndices()));
		static std::vector<VertexAttribPointerSpecification> Spec = {
				{3,GL_FLOAT,false},
				{3,GL_FLOAT,false},
				{2,GL_FLOAT,false}
		};
		Vao.SetVertexAttribArray(Spec);
		Renderable2D::SetVertexArray(std::move(Vao));

		Renderable2D::SetMaterial(Ermine::Material(std::filesystem::path("Shader/Actor2DBaseMaterial.json")));
	}
}
