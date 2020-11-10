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
	{
		MUTEXGAURD(Ermine::MutexLevel::Actor2D);
	}

	//Sorely Missed Copy Constructor
	Actor2D::Actor2D(Actor2D & rhs)
		:
		ImageBase(rhs.GetSprite()),
		MovableObject(rhs)
	{}

	//Copy Operator
	Actor2D& Actor2D::operator=(Actor2D& rhs)
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
		MUTEXGAURD(Ermine::MutexLevel::Actor2D);
		auto Gaurd = rhs.GetErmineMutexGaurd();

		rhs.SetSprite(nullptr);
	}

	//Sorely Missed Move Operator 
	Actor2D& Actor2D::operator=(Actor2D&& rhs)
	{
		MUTEXGAURD(Ermine::MutexLevel::Actor2D);
		auto Gaurd = rhs.GetErmineMutexGaurd();

		ImageBase::operator=(std::move(rhs));
		MovableObject::operator=(std::move(rhs));

		return *this;
	}
#pragma endregion Constructors

#pragma region GeneratorFunctions
	std::shared_ptr<Actor2D> Actor2D::GenerateActor2D(std::filesystem::path TexturePath)
	{
		auto Sprite = GenSprite(TexturePath, glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f));
		std::shared_ptr<Ermine::Actor2D> Act = std::make_shared<Ermine::Actor2D>(Sprite);
		return Act;
	}

	std::shared_ptr<Actor2D> Actor2D::GenerateActor2D(std::filesystem::path TexturePath, glm::vec2 ActorScreenLocation, float Rotation, glm::vec2 Scale)
	{
		glm::mat4 ModelMatrix = GenModelMatrix(ActorScreenLocation, Rotation, Scale);
		auto Spr = GenSprite(TexturePath, glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f));
		std::shared_ptr<Ermine::Actor2D> Act = std::make_shared<Ermine::Actor2D>(Spr,ModelMatrix);
		return Act;
	}

	std::shared_ptr<Actor2D> Actor2D::GenerateActor2D(std::shared_ptr<Sprite> Spr)
	{
		std::shared_ptr<Ermine::Actor2D> Act = std::make_shared<Ermine::Actor2D>(Spr);
		return Act;
	}

	std::shared_ptr<Actor2D> Actor2D::GenerateActor2D(std::shared_ptr<Sprite> Spr, glm::vec2 ActorScreenLocation, float Rotation, glm::vec2 Scale)
	{
		glm::mat4 ModelMatrix = GenModelMatrix(ActorScreenLocation, Rotation, Scale);
		std::shared_ptr<Ermine::Actor2D> Act = std::make_shared<Ermine::Actor2D>(Spr,ModelMatrix);
		return Act;
	}
#pragma endregion

	std::vector<float> Actor2D::CalculateModelSpaceVertexes()
	{
		MUTEXGAURD(Ermine::MutexLevel::Actor2D);

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
		MUTEXGAURD(Ermine::MutexLevel::Actor2D);
		return MovableObject::GetScreenLocation();
	}
	void Actor2D::SetActorPosition(glm::vec2 ActorPosition)
	{
		MUTEXGAURD(Ermine::MutexLevel::Actor2D);
		MovableObject::SetPosition(ActorPosition);
	}
	glm::vec2 Actor2D::GetActorVelocity()
	{
		MUTEXGAURD(Ermine::MutexLevel::Actor2D);
		return MovableObject::GetVelocity();
	}
	void Actor2D::SetActorVelocity(glm::vec2 ActorVelocity)
	{
		MUTEXGAURD(Ermine::MutexLevel::Actor2D);
		MovableObject::SetVelocity(ActorVelocity);
	}
	float Actor2D::GetAngularVelocity(bool Degrees)
	{
		MUTEXGAURD(Ermine::MutexLevel::Actor2D);
		return MovableObject::GetAngularVelocity(Degrees);
	}
	void Actor2D::SetAngularVelocity(float AngularVelocity, bool Degrees)
	{
		MUTEXGAURD(Ermine::MutexLevel::Actor2D);
		MovableObject::SetAngularVelocity(AngularVelocity, Degrees);
	}
#pragma endregion MovableActorImplementation

	std::shared_ptr<Ermine::Sprite> Actor2D::GenSprite(std::filesystem::path TexturePath, glm::vec2 BottomLeft, glm::vec2 TopRight)
	{
		auto Cache = Ermine::GlobalTextureCache::Get();
		std::shared_ptr<Ermine::Texture> Tex = Cache->GetTextureFromFile(TexturePath);

		std::shared_ptr<Ermine::Sprite> Sprite = Ermine::Sprite::GenerateSprite(Tex,BottomLeft,TopRight);

		return Sprite;
	}

	glm::mat4 Actor2D::GenModelMatrix(glm::vec2 ActorScreenLocation, float Rotation, glm::vec2 Scale)
	{
		glm::mat4 ModelMatrix = glm::mat4(1.0f);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(ActorScreenLocation, 0.0f));
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians<float>(Rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(Scale, 1.0f));

		return ModelMatrix;
	}


}