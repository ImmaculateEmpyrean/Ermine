#include "stdafx.h"
#include "MovableObject.h"

namespace Ermine
{
	MovableObject::MovableObject()
	{
		TranslationMatrix = glm::mat4(1.0f);
		RotationMatrix = glm::mat4(1.0f);
		ScaleMatrix = glm::mat4(1.0f);

		RecievedModelMatrix = glm::mat4(1.0f);
	}
	MovableObject::MovableObject(glm::mat4 ModelMatrix)
		:
		RecievedModelMatrix(ModelMatrix)
	{
		TranslationMatrix = glm::mat4(1.0f);
		RotationMatrix = glm::mat4(1.0f);
		ScaleMatrix = glm::mat4(1.0f);
	}


	glm::mat4 MovableObject::GetModelMatrix()
	{
		return RecievedModelMatrix * ScaleMatrix * RotationMatrix * TranslationMatrix;
	}

	glm::mat4 MovableObject::GetTranslationMatrix()
	{
		return TranslationMatrix;
	}
	glm::mat4 MovableObject::GetRotationMatrix()
	{
		return RotationMatrix;
	}
	glm::mat4 MovableObject::GetScaleMatrix()
	{
		return ScaleMatrix;
	}


	void MovableObject::Translate(float x, float y)
	{
		TranslationMatrix = glm::translate(TranslationMatrix, { x,y,0.0f });
	}
	void MovableObject::Translate(glm::vec2 TranslateByHowMuch)
	{
		TranslationMatrix = glm::translate(TranslationMatrix, { TranslateByHowMuch.x,TranslateByHowMuch.y,0.0f });
	}
	void MovableObject::ClearTranslations()
	{
		TranslationMatrix = glm::mat4(1.0f);
	}

	void MovableObject::Rotate(float Angle, bool IsInPI)
	{
		if (!IsInPI) //There Might Be Slight Error Here.. Check When Free.. If Its Already In radians Why Bother Converting iNto Radian..
			RotationMatrix = glm::rotate(RotationMatrix, glm::radians(Angle), glm::vec3(0.0, 0.0, 1.0)); //Rotate On Z Axis Since This is 2D..
		else
			RotationMatrix = glm::rotate(RotationMatrix, Angle, glm::vec3(0.0, 0.0, 1.0)); //Rotate On Z Axis Since This is 2D..
	}
	void MovableObject::ClearRotations()
	{
		RotationMatrix = glm::mat4(1.0f);
	}

	void MovableObject::Scale(float x, float y)
	{
		ScaleMatrix = glm::scale(ScaleMatrix, { x,y,1.0f });
	}
	void MovableObject::Scale(glm::vec2 ScaleByHowMuch)
	{
		ScaleMatrix = glm::scale(ScaleMatrix, { ScaleByHowMuch.x,ScaleByHowMuch.y,1.0f });
	}
	void MovableObject::ClearScale()
	{
		ScaleMatrix = glm::mat4(1.0f);
	}
}
