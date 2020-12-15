#include "stdafx.h"
#include "Actor2DShape.h"

namespace Ermine
{
#pragma region Constructor
	Actor2DShape::Actor2DShape()
		:
		MovableObject(glm::vec2(0.0f,0.0f)),
		ShapeBase()
	{}

	Actor2DShape::Actor2DShape(glm::vec2 SpawnLocation)
		:
		MovableObject(SpawnLocation),
		ShapeBase()
	{}

	Actor2DShape::Actor2DShape(std::vector<Ermine::VertexTextured> Points)
		:
		MovableObject(glm::vec2(0.0f,0.0f)),
		ShapeBase(Points)
	{}

	Actor2DShape::Actor2DShape(glm::vec2 SpawnLocation, std::vector<Ermine::VertexTextured> Points)
		:
		MovableObject(SpawnLocation),
		ShapeBase(Points)
	{}

	Actor2DShape::Actor2DShape(glm::vec2 SpawnPosition, std::vector<Ermine::VertexTextured> Points, std::shared_ptr<Ermine::Sprite> ActorSprite)
		:
		MovableObject(SpawnPosition),
		ShapeBase(ActorSprite,Points)
	{}


	Actor2DShape::Actor2DShape(Actor2DShape& rhs)
		:
		MovableObject(rhs),
		ShapeBase(rhs)
	{}
	Actor2DShape& Actor2DShape::operator=(Actor2DShape& rhs)
	{
		auto ForeignLock = rhs.GetObjectMutex();
		auto Lock = GetObjectMutex();

		MovableObject::operator=(rhs);
		ShapeBase::operator=(rhs);

		return *this;
	}
	Actor2DShape::Actor2DShape(Actor2DShape&& rhs)
		:
		MovableObject(std::move(rhs)),
		ShapeBase(std::move(rhs))
	{}
	Actor2DShape& Actor2DShape::operator=(Actor2DShape&& rhs)
	{
		auto ForeignLock = rhs.GetObjectMutex();
		auto Lock		 = GetObjectMutex();

		MovableObject::operator=(std::move(rhs));
		ShapeBase::operator=(std::move(rhs));

		return *this;
	}

	Actor2DShape::~Actor2DShape()
	{}
#pragma endregion

	std::shared_ptr<Ermine::Actor2DShape> Actor2DShape::Generate(glm::vec2 SpawnLocation, std::vector<VertexTextured> Vertices, std::shared_ptr<Ermine::Sprite> Spr)
	{
		return std::shared_ptr<Ermine::Actor2DShape>(new Ermine::Actor2DShape(SpawnLocation, Vertices, Spr));
	}
	glm::mat4 Actor2DShape::GetModelMatrix()
	{
		return MovableObject::GetModelMatrix();
	}

#pragma region ImovableActorImplementation
	glm::vec2 Actor2DShape::GetActorPosition()
	{
		auto Lock = GetObjectMutex();
		return MovableObject::GetScreenLocation();
	}

	void Actor2DShape::SetActorPosition(glm::vec2 ActorPosition)
	{
		auto Lock = GetObjectMutex();
		MovableObject::SetPosition(ActorPosition);
	}

	glm::vec2 Actor2DShape::GetActorVelocity()
	{
		auto Lock = GetObjectMutex();
		return MovableObject::GetVelocity();
	}

	void Actor2DShape::SetActorVelocity(glm::vec2 ActorVelocity)
	{
		auto Lock = GetObjectMutex();
		MovableObject::SetVelocity(ActorVelocity);
	}

	float Actor2DShape::GetAngularVelocity(bool Degrees)
	{
		auto Lock = GetObjectMutex();
		return MovableObject::GetAngularVelocity(Degrees);
	}

	void Actor2DShape::SetAngularVelocity(float AngularVelocity, bool Degrees)
	{
		auto Lock = GetObjectMutex();
		MovableObject::SetAngularVelocity(AngularVelocity, Degrees);
	}

#pragma endregion

	void Ermine::Actor2DShape::OnUpdateTickEventRecieved()
	{
		//Start For Testing..//
		MovableObject::Translate({ 1.0f,0.0f });
		//Ended For Testing..//

		//We Will Call The Movable Objects Update Here If In The Future Movable Object Supports Things Like Say velocity.. :>
	}
}