#pragma once
#include<iostream>
#include<vector>
#include<string>

#include<glm.hpp>

namespace Ermine
{
	class MovableObject
	{
	public:
		//This Can Exist But It Is Not Set To Default..
		MovableObject(); 

		//Send In a Default Model Matrix So That We Can Default To it..
		MovableObject(glm::mat4 ModelMatrix);

	public:
		virtual glm::mat4 GetModelMatrix();

		glm::mat4 GetTranslationMatrix();
		glm::mat4 GetRotationMatrix();
		glm::mat4 GetScaleMatrix();

		void Translate(float x, float y);
		void Translate(glm::vec2 TranslateByHowMuch);
		void ClearTranslations();

		//Just Pass In True If The Angle Is Actually Pi Most Probably It Is In Degrees...(if false is passed then i assume DEGREES).
		void Rotate(float Angle, bool IsInPI = false);
		void ClearRotations();

		void Scale(float x, float y);
		void Scale(glm::vec2 ScaleByHowMuch);
		void ClearScale();

	public:

	protected:

	protected:

	private:

	public:
		//The Model Matrix Recieved By The Constructor If Any.. This Is The Default Matrix We Will Default to In The Case Clear Is Called..
		glm::mat4 RecievedModelMatrix;

		glm::mat4 TranslationMatrix;
		glm::mat4 RotationMatrix;
		glm::mat4 ScaleMatrix;
	};
}