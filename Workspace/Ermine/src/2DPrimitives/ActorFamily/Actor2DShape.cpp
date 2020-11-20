#include "stdafx.h"
#include "Actor2DShape.h"

namespace Ermine
{
#pragma region Constructor
	Actor2DShape::Actor2DShape()
		:
		MovableObject(glm::vec2(0.0f,0.0f)),
		ImageBase(),
		RenderableShapeComponent()
	{}

	Actor2DShape::Actor2DShape(glm::vec2 SpawnLocation)
		:
		MovableObject(SpawnLocation),
		ImageBase(),
		RenderableShapeComponent()
	{}

	Actor2DShape::Actor2DShape(std::vector<Ermine::VertexTextured> Points)
		:
		MovableObject(glm::vec2(0.0f,0.0f)),
		ImageBase(),
		RenderableShapeComponent()
	{
		auto Lock = Actor2DBase::GetObjectMutex();
		Vertexes = Points;
	}

	Actor2DShape::Actor2DShape(glm::vec2 SpawnLocation, std::vector<Ermine::VertexTextured> Points)
		:
		MovableObject(SpawnLocation),
		ImageBase(),
		RenderableShapeComponent()
	{
		auto Lock = Actor2DBase::GetObjectMutex();
		Vertexes = Points;
	}


	Actor2DShape::Actor2DShape(Actor2DShape& rhs)
	{
		auto ForeignLock = rhs.GetObjectMutex();
		auto Lock = GetObjectMutex();

		MovableObject::operator=(rhs);
		ImageBase::operator=(rhs);
		RenderableShapeComponent::operator=(rhs);

		Vertexes = rhs.Vertexes;
	}
	Actor2DShape& Actor2DShape::operator=(Actor2DShape& rhs)
	{
		auto ForeignLock = rhs.GetObjectMutex();
		auto Lock = GetObjectMutex();

		MovableObject::operator=(rhs);
		ImageBase::operator=(rhs);
		RenderableShapeComponent::operator=(rhs);

		Vertexes = rhs.Vertexes;

		return *this;
	}
	Actor2DShape::Actor2DShape(Actor2DShape&& rhs)
	{
		auto ForeignLock = rhs.GetObjectMutex();
		auto Lock = GetObjectMutex();

		MovableObject::operator=(std::move(rhs));
		ImageBase::operator=(std::move(rhs));
		RenderableShapeComponent::operator=(std::move(rhs));

		Vertexes = std::move(rhs.Vertexes);
	}
	Actor2DShape& Actor2DShape::operator=(Actor2DShape&& rhs)
	{
		auto ForeignLock = rhs.GetObjectMutex();
		auto Lock = GetObjectMutex();

		MovableObject::operator=(std::move(rhs));
		ImageBase::operator=(std::move(rhs));
		RenderableShapeComponent::operator=(std::move(rhs));

		Vertexes = std::move(rhs.Vertexes);

		return *this;
	}

	Actor2DShape::~Actor2DShape()
	{
		//The Destructor Is Empty For Now.. USed For Virtual Calling Probably..
	}
#pragma endregion

#pragma region Exposure

	unsigned int Actor2DShape::GetNumberOfPointsHeld()
	{
		//Get Lock Since We Are Dealing With Memory
		auto Lock = GetObjectMutex();

		return Vertexes.size();
	}
	VertexTextured& Actor2DShape::GetPoint(unsigned int Index)
	{
		//Get Lock Since We Are Dealing With Memory
		auto Lock = GetObjectMutex();

		return Vertexes[Index];
	}

	void Actor2DShape::AddPoint(Ermine::VertexTextured Point)
	{
		//Get Lock Since We Are Dealing With Memory
		auto Lock = GetObjectMutex();

		Vertexes.emplace_back(Point);
	}

	void Actor2DShape::DeletePoint(unsigned int Index)
	{
		//Get Lock Since We Are Dealing With Memory
		auto Lock = GetObjectMutex();

		// Erase Using The Iterator Of Said Array..
		Vertexes.erase(Vertexes.begin() + Index);
	}

#pragma endregion

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


	void Actor2DShape::RefreshRenderable()
	{
		Renderable2D::SetVertexArray();
	}

}