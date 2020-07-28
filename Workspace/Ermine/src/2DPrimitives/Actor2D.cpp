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
		HelperInitializeRenderable2D();
	}
	Actor2D::Actor2D(std::shared_ptr<Sprite> Spr, glm::mat4 ModelMatrix)
		:
		MovableObject(ModelMatrix),
		Actorsprite(Spr)
	{
		HelperInitializeRenderable2D();
	}

	Actor2D::~Actor2D()
	{}


	std::vector<float> Actor2D::GetModelSpaceCoordinates()
	{
		std::vector<float> ModelCoordinates = Quad::GetModelCoordinates();

		//Top Right..
		ModelCoordinates[6] = Actorsprite->GetTopRightUV().x;
		ModelCoordinates[7] = Actorsprite->GetBottomLeftUV().y;//Actorsprite->GetTopRightUV().y;

		//Bottom Right
		ModelCoordinates[14] = Actorsprite->GetTopRightUV().x;//ModelCoordinates[14] = Actorsprite->GetBottomLeftUV().x;
		ModelCoordinates[15] = Actorsprite->GetTopRightUV().y;//Actorsprite->GetBottomLeftUV().y;

		//Bottom Left
		ModelCoordinates[22] = Actorsprite->GetBottomLeftUV().x;
		ModelCoordinates[23] = Actorsprite->GetTopRightUV().y;//Actorsprite->GetBottomLeftUV().y;

		//Top Left
		ModelCoordinates[30] = Actorsprite->GetBottomLeftUV().x;
		ModelCoordinates[31] = Actorsprite->GetBottomLeftUV().y;//Actorsprite->GetTopRightUV().y;

		return ModelCoordinates;
	}

	std::vector<uint32_t> Actor2D::GetModelSpaceIndices()
	{
		return Quad::GetModelIndices();
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
