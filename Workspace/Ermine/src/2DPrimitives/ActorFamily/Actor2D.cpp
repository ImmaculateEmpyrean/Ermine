#include "stdafx.h"
#include "Actor2D.h"

#include "2DPrimitives/Constructs/Quad.h"
#include "2DPrimitives/Constructs/VertexTextured.h"

namespace Ermine
{
#pragma region Constructors
	Actor2D::Actor2D(std::shared_ptr<Sprite> Spr)
		:
		ImageBase(Spr)
	{
		RefreshRenderable2D();
	}
	Actor2D::Actor2D(std::vector<std::shared_ptr<Sprite>> SpriteBuffer)
		:
		ImageBase(std::make_shared<SpriteBook>("SpriteBuffer", SpriteBuffer))
	{
		RefreshRenderable2D();
	}

	Actor2D::Actor2D(std::shared_ptr<Sprite> Spr, glm::mat4 ModelMatrix)
		:
		MovableObject(ModelMatrix),
		ImageBase(Spr)
	{
		RefreshRenderable2D();
	}

	Actor2D::Actor2D(std::vector<std::shared_ptr<Sprite>> SpriteBuffer, glm::mat4 ModelMatrix)
		:
		MovableObject(ModelMatrix),
		ImageBase(std::make_shared<SpriteBook>("SpriteBuffer", SpriteBuffer))
	{
		RefreshRenderable2D();
	}

	Actor2D::~Actor2D()
	{}

	//Sorely Missed Copy Constructor
	Actor2D::Actor2D(const Actor2D & rhs)
		:
		ImageBase(rhs.GetSprite()),
		MovableObject(rhs)
	{}

	//Copy Operator
	Actor2D& Actor2D::operator=(const Actor2D& rhs)
	{
		ImageBase::operator=(rhs);
		MovableObject::operator=(rhs);

		return *this;
	}

	//Sorely Missed Move Constructor
	Actor2D::Actor2D(Actor2D&& rhs)
		:
		ImageBase(std::move(rhs.GetSprite())),
		MovableObject(std::move(rhs))
	{
		rhs.SetSprite(nullptr);
	}

	//Sorely Missed Move Operator 
	Actor2D& Actor2D::operator=(Actor2D&& rhs)
	{
		ImageBase::operator=(std::move(rhs));
		MovableObject::operator=(std::move(rhs));

		return *this;
	}

#pragma endregion Constructors

	std::vector<float> Actor2D::CalculateModelSpaceVertexes()
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

		TopRight.SetPositonCoordinates(TopRightPos4);
		BottomRight.SetPositonCoordinates(BottomRightPos4);
		BottomLeft.SetPositonCoordinates(BottomLeftPos4);
		TopLeft.SetPositonCoordinates(TopLeftPos4);

		std::shared_ptr<Ermine::Sprite> Actorsprite = GetSprite();
		TopRight.   SetVertexUV(glm::vec2(Actorsprite->GetTopRightUV().x  , Actorsprite->GetBottomLeftUV().y));
		BottomRight.SetVertexUV(glm::vec2(Actorsprite->GetTopRightUV().x  , Actorsprite->GetTopRightUV().  y));
		BottomLeft. SetVertexUV(glm::vec2(Actorsprite->GetBottomLeftUV().x, Actorsprite->GetTopRightUV().  y));
		TopLeft.    SetVertexUV(glm::vec2(Actorsprite->GetBottomLeftUV().x, Actorsprite->GetBottomLeftUV().y));

		std::vector<float> ModelCoordinates;
		ModelCoordinates = TopRight;
		ModelCoordinates = ModelCoordinates + BottomRight;
		ModelCoordinates = ModelCoordinates + BottomLeft;
		ModelCoordinates = ModelCoordinates + TopLeft;

		return ModelCoordinates;
	}

#pragma region MovableActorImplementation
	glm::vec2 Actor2D::GetActorPosition()
	{
		return MovableObject::GetScreenLocation();
	}
	void Actor2D::SetActorPosition(glm::vec2 ActorPosition)
	{
		MovableObject::SetPosition(ActorPosition);
	}
	glm::vec2 Actor2D::GetActorVelocity()
	{
		return MovableObject::GetVelocity();
	}
	void Actor2D::SetActorVelocity(glm::vec2 ActorVelocity)
	{
		MovableObject::SetVelocity(ActorVelocity);
	}
	float Actor2D::GetAngularVelocity(bool Degrees)
	{
		return MovableObject::GetAngularVelocity(Degrees);
	}
	void Actor2D::SetAngularVelocity(float AngularVelocity, bool Degrees)
	{
		MovableObject::SetAngularVelocity(AngularVelocity, Degrees);
	}
#pragma endregion MovableActorImplementation

}