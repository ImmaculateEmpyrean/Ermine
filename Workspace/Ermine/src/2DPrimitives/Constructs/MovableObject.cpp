#include "stdafx.h"
#include "MovableObject.h"

#include <gtx/matrix_decompose.hpp>

namespace Ermine
{
#pragma region Constructors

	MovableObject::MovableObject()
	{
		HelperRecalculateCache();
	}
	MovableObject::MovableObject(glm::vec2 SpawnPosition)
		:
		Position(SpawnPosition)
	{
		HelperRecalculateCache();
	}
	MovableObject::MovableObject(glm::vec2 SpawnPosition, float Rotation)
		:
		Position(SpawnPosition),
		Rotation(Rotation)
	{
		HelperRecalculateCache();
	}
	MovableObject::MovableObject(glm::vec2 SpawnPosition, float Rotation, glm::vec2 Scale)
		:
		Position(SpawnPosition),
		Rotation(Rotation),
		scale(Scale)
	{
		HelperRecalculateCache();
	}
	MovableObject::MovableObject(float Rotation, glm::vec2 Scale)
		:
		Rotation(Rotation),
		scale(Scale)
	{
		HelperRecalculateCache();
	}
	MovableObject::MovableObject(glm::mat4 ModelMatrix)
	{

		glm::vec3 scale;
		glm::quat rotation;
		glm::vec3 translation;
		glm::vec3 skew;
		glm::vec4 perspective;
		glm::decompose(ModelMatrix, scale, rotation, translation, skew, perspective);

		Position.x = translation.x;
		Position.y = translation.y;

		Rotation = rotation.z;

		this->scale.x = scale.x;
		this->scale.y = scale.y;

		HelperRecalculateCache();
	}

	MovableObject::~MovableObject()
	{}

	MovableObject::MovableObject(const MovableObject& rhs)
	{
		//The Helper Function Hopefully Functions Properly..
		HelperCopy(rhs);
	}
	MovableObject& MovableObject::operator=(const MovableObject& rhs)
	{
		//The Helper Function Hopefully Functions Properly..
		HelperCopy(rhs);
		return *this;
	}

	MovableObject::MovableObject(MovableObject&& rhs)
	{
		//The Helper Function Hopefully Functions Properly..
		HelperMove(std::move(rhs));
	}
	MovableObject& MovableObject::operator=(MovableObject&& rhs)
	{
		//The Helper Function Hopefully Functions Properly..
		HelperMove(std::move(rhs));
		return *this;
	}


	void MovableObject::HelperCopy(const MovableObject& rhs)
	{
		CacheValid = rhs.CacheValid;
		ModelMatrixCacheValid = rhs.ModelMatrixCacheValid;

		ModelMatrix = rhs.ModelMatrix;

		TranslationMatrix = rhs.TranslationMatrix;
		RotationMatrix    = rhs.RotationMatrix;
		ScaleMatrix = rhs.ScaleMatrix;

		Position = rhs.Position;

		Rotation = rhs.Rotation;

		scale = rhs.scale;

		Velocity = rhs.Velocity;
		AngularVelocityInDegrees = rhs.AngularVelocityInDegrees;

	}

	void MovableObject::HelperMove(MovableObject&& rhs)
	{
		CacheValid = std::move(rhs.CacheValid);
		ModelMatrixCacheValid = std::move(rhs.ModelMatrixCacheValid);

		ModelMatrix = std::move(rhs.ModelMatrix);

		TranslationMatrix = std::move(rhs.TranslationMatrix);
		RotationMatrix = std::move(rhs.RotationMatrix);
		ScaleMatrix = std::move(rhs.ScaleMatrix);

		Position = std::move(rhs.Position);

		Rotation = std::move(rhs.Rotation);

		scale = std::move(rhs.scale);

		Velocity = std::move(rhs.Velocity);
		AngularVelocityInDegrees = std::move(rhs.AngularVelocityInDegrees);

	}

#pragma endregion Constructors

#pragma region Getters

	glm::mat4 MovableObject::GetModelMatrix()
	{
		

		if (ModelMatrixCacheValid == true)
			return ModelMatrix;
		else
		{
			HelperRecalculateModelMatrix();
			//Recursively Call To Check If The Cache Is Valid Yet (It Will Return True On The Second Try Hopefully.)
			return GetModelMatrix();
		}
	}

	glm::mat4 MovableObject::GetTranslationMatrix()
	{
		if (CacheValid == true)
			return TranslationMatrix;
		else
		{
			HelperRecalculateCache();
			//Recursively Call To Check If The Cache Is Valid Yet (It Will Return True On The Second Try Hopefully.)
			return GetTranslationMatrix();
		}
	}

	glm::mat4 MovableObject::GetRotationMatrix()
	{
		if (CacheValid == true)
			return RotationMatrix;
		else
		{
			HelperRecalculateCache();
			//Recursively Call To Check If The Cache Is Valid Yet (It Will Return True On The Second Try Hopefully.)
			return GetRotationMatrix();
		}
	}

	glm::mat4 MovableObject::GetScaleMatrix()
	{
		if (CacheValid == true)
			return ScaleMatrix;
		else
		{
			HelperRecalculateCache();
			//Recursively Call To Check If The Cache Is Valid Yet (It Will Return True On The Second Try Hopefully.)
			return GetScaleMatrix();
		}
	}

	glm::vec2 MovableObject::GetScreenLocation()
	{
		return Position;
	}

	glm::vec2 MovableObject::GetVelocity()
	{
		return Velocity;
	}

	float MovableObject::GetRotation()
	{
		return Rotation;
	}

	float MovableObject::GetAngularVelocity(bool Degrees)
	{
		if (Degrees == true)
			return AngularVelocityInDegrees;
		else
			return glm::radians<float>(AngularVelocityInDegrees);
	}

	glm::vec2 MovableObject::GetScale()
	{
		return scale;
	}

#pragma endregion Getters

#pragma region Setters

	void MovableObject::SetPosition(float x, float y)
	{
		SetPosition(glm::vec2(x, y));
	}

	void MovableObject::SetPosition(glm::vec2 NewPos)
	{
		Position = NewPos;

		CacheValid = false;
		ModelMatrixCacheValid = false;
	}

	void MovableObject::Translate(float x, float y)
	{
		Translate(glm::vec2(x, y));
	}

	void MovableObject::Translate(glm::vec2 TranslateByHowMuch)
	{
		Position = Position + TranslateByHowMuch;

		CacheValid = false;
		ModelMatrixCacheValid = false;
	}

	void MovableObject::ClearTranslations()
	{
		Position = glm::vec2(0.0f, 0.0f);

		CacheValid = false;
		ModelMatrixCacheValid = false;
	}

	void MovableObject::SetVelocity(float x, float y)
	{
		SetVelocity(glm::vec2(x, y));
	}

	void MovableObject::SetVelocity(glm::vec2 Velocity)
	{
		this->Velocity = Velocity;

		CacheValid = false;
		ModelMatrixCacheValid = false;
	}

	void MovableObject::ClearVelocity()
	{
		Velocity = glm::vec2(0.0f);
	}

	void MovableObject::Rotate(float Angle, bool Degrees)
	{
		if (Degrees == true)
			Rotation = Rotation + Angle;
		else Rotation = Rotation + glm::degrees<float>(Angle);

		if (Rotation >= 360.0f)
			Rotation = Rotation - 360.0f;

		CacheValid = false;
		ModelMatrixCacheValid = false;
	}

	void MovableObject::SetRotation(float Angle, bool Degrees)
	{
		if (Degrees == true)
			Rotation = Angle;
		else Rotation = glm::degrees<float>(Angle);

		if (Rotation >= 360.0f)
			Rotation = Rotation - 360.0f;

		CacheValid = false;
		ModelMatrixCacheValid = false;
	}

	void MovableObject::ClearRotations()
	{
		Rotation = 0.0f;

		CacheValid = false;
		ModelMatrixCacheValid = false;
	}

	void MovableObject::SetAngularVelocity(float Angle, bool Degrees)
	{
		if (Degrees == false)
			Angle = glm::degrees<float>(Angle);

		AngularVelocityInDegrees = Angle;

		CacheValid = false;
		ModelMatrixCacheValid = false;
	}

	void MovableObject::ClearAngularVelocity()
	{
		AngularVelocityInDegrees = 0.0f;
	}

	void MovableObject::SetScale(float x, float y)
	{
		SetScale(glm::vec2(x, y));
	}

	void MovableObject::SetScale(glm::vec2 Scale)
	{
		scale = Scale;

		CacheValid = false;
		ModelMatrixCacheValid = false;
	}

	void MovableObject::Scale(float x, float y)
	{
		Scale(glm::vec2(x, y));
	}

	void MovableObject::Scale(glm::vec2 ScaleByHowMuch)
	{
		scale = scale + ScaleByHowMuch;

		CacheValid = false;
		ModelMatrixCacheValid = false;
	}

	void MovableObject::ClearScale()
	{
		scale = glm::vec2(0.0f, 0.0f);

		CacheValid = false;
		ModelMatrixCacheValid = false;
	}

#pragma endregion Setters

#pragma region Helpers

	void Ermine::MovableObject::HelperRecalculateCache()
	{
		TranslationMatrix = glm::mat4(1.0f);
		TranslationMatrix = glm::translate(TranslationMatrix, glm::vec3(Position, 0.0f));

		RotationMatrix = glm::mat4(1.0f);
		RotationMatrix = glm::rotate(RotationMatrix, glm::radians<float>(Rotation), glm::vec3(0.0f, 0.0f, 1.0f));

		ScaleMatrix = glm::mat4(1.0f);
		ScaleMatrix = glm::scale(ScaleMatrix, glm::vec3(scale, 1.0f));

		HelperRecalculateModelMatrix();

		CacheValid = true;
	}

	void Ermine::MovableObject::HelperRecalculateModelMatrix()
	{
		ModelMatrix = glm::mat4(1.0f);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(Position, 0.0f));
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians<float>(Rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(scale, 1.0f));

		ModelMatrixCacheValid = true;
	}

	void MovableObject::Update(float DeltaTime)
	{
		STDOUTDefaultLog_Trace("Update Called in Movable Object");
	}

#pragma endregion Helpers
}