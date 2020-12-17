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

	Actor2DShape::Actor2DShape(std::vector<std::shared_ptr<Sprite>> SpriteBuffer, glm::mat4 ModelMatrix, std::vector<VertexTextured> Points)
		:
		MovableObject(ModelMatrix),
		ShapeBase(SpriteBuffer,Points)
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





	void Ermine::Actor2DShape::OnUpdateTickEventRecieved()
	{
		auto Lock = Object::GetObjectMutex();

		//Update The Movable Object Every Frame Of The Engine
		MovableObject::Update();

		//MovableObject::Scale(1.01f, 1.01f);
		//MovableObject::Translate(1.0f, 0.0f);
		STDOUTDefaultLog_Info("ActorShape Tick Working..");
	}

#pragma region MovableObjectOverrides
	float Actor2DShape::GetRotation()
	{
		auto Lock = Object::GetObjectMutex();
		return MovableObject::GetRotation();
	}
	float Actor2DShape::GetAngularVelocity(bool Degrees)
	{
		auto lock = Object::GetObjectMutex();
		return MovableObject::GetAngularVelocity(Degrees);
	}
	glm::vec2 Actor2DShape::GetScale()
	{
		auto Lock = Object::GetObjectMutex();
		return MovableObject::GetScale();
	}
	void Actor2DShape::SetPosition(float x, float y)
	{
		auto Lock = Object::GetObjectMutex();
		MovableObject::SetPosition(x, y);
	}
	void Actor2DShape::SetPosition(glm::vec2 NewPos)
	{
		auto Lock = Object::GetObjectMutex();
		MovableObject::SetPosition(NewPos);
	}
	void Actor2DShape::Translate(float x, float y)
	{
		auto Lock = Object::GetObjectMutex();
		MovableObject::Translate(x, y);
	}
	void Actor2DShape::Translate(glm::vec2 TranslateByHowMuch)
	{
		auto Lock = Object::GetObjectMutex();
		MovableObject::Translate(TranslateByHowMuch);
	}
	void Actor2DShape::ClearTranslations()
	{
		auto Lock = Object::GetObjectMutex();
		MovableObject::ClearTranslations();
	}
	void Actor2DShape::SetVelocity(float x, float y)
	{
		auto Lock = Object::GetObjectMutex();
		MovableObject::SetVelocity(x, y);
	}
	void Actor2DShape::SetVelocity(glm::vec2 Velocity)
	{
		auto Lock = Object::GetObjectMutex();
		MovableObject::SetVelocity(Velocity);
	}
	void Actor2DShape::ClearVelocity()
	{
		auto Lock = Object::GetObjectMutex();
		MovableObject::ClearVelocity();
	}
	void Actor2DShape::Rotate(float Angle, bool Degrees)
	{
		auto Lock = Object::GetObjectMutex();
		MovableObject::Rotate(Angle, Degrees);
	}
	void Actor2DShape::SetRotation(float Angle, bool Degrees)
	{
		auto Lock = Object::GetObjectMutex();
		MovableObject::SetRotation(Angle, Degrees);
	}
	void Actor2DShape::ClearRotations()
	{
		auto Lock = Object::GetObjectMutex();
		MovableObject::ClearRotations();
	}
	void Actor2DShape::SetAngularVelocity(float AngularVelocity, bool Degrees)
	{
		auto lock = Object::GetObjectMutex();
		MovableObject::SetAngularVelocity(AngularVelocity, Degrees);
	}
	void Actor2DShape::ClearAngularVelocity()
	{
		auto Lock = Object::GetObjectMutex();
		MovableObject::ClearAngularVelocity();
	}
	void Actor2DShape::SetScale(float x, float y)
	{
		auto Lock = Object::GetObjectMutex();
		MovableObject::SetScale(x, y);
	}
	void Actor2DShape::SetScale(glm::vec2 Scale)
	{
		auto Lock = Object::GetObjectMutex();
		MovableObject::SetScale(Scale);
	}
	void Actor2DShape::Scale(float x, float y)
	{
		auto Lock = Object::GetObjectMutex();
		MovableObject::Scale(x, y);
	}
	void Actor2DShape::Scale(glm::vec2 ScaleByHowMuch)
	{
		auto Lock = Object::GetObjectMutex();
		MovableObject::Scale(ScaleByHowMuch);
	}
	void Actor2DShape::ClearScale()
	{
		auto Lock = Object::GetObjectMutex();
		MovableObject::ClearScale();
	}
	glm::mat4 Actor2DShape::GetModelMatrix()
	{
		auto Lock = Object::GetObjectMutex();
		return MovableObject::GetModelMatrix();
	}
	glm::mat4 Actor2DShape::GetTranslationMatrix()
	{
		auto Lock = Object::GetObjectMutex();
		return MovableObject::GetTranslationMatrix();
	}
	glm::mat4 Actor2DShape::GetRotationMatrix()
	{
		auto Lock = Object::GetObjectMutex();
		return MovableObject::GetRotationMatrix();
	}
	glm::mat4 Actor2DShape::GetScaleMatrix()
	{
		auto Lock = Object::GetObjectMutex();
		return MovableObject::GetScaleMatrix();
	}
	glm::vec2 Actor2DShape::GetScreenLocation()
	{
		auto Lock = Object::GetObjectMutex();
		return MovableObject::GetScreenLocation();
	}
	glm::vec2 Actor2DShape::GetVelocity()
	{
		auto Lock = Object::GetObjectMutex();
		return MovableObject::GetVelocity();
	}
#pragma endregion
}