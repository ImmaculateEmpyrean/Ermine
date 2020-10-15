#pragma once
#include<iostream>
#include<vector>
#include<string>

#include <glm.hpp>
/*
	This Is An Interface Class Use This To Simply Call Some Dunctions In The Future..
*/

namespace Ermine
{
	class MovableActor
	{
	public:
		MovableActor() = default;

	public:
		//Start Main Interface Functions//

		virtual glm::vec2 GetActorPosition() = 0;
		virtual void SetActorPosition(glm::vec2 ActorPosition) = 0;

		virtual glm::vec2 GetActorVelocity() = 0;
		virtual void SetActorVelocity(glm::vec2 ActorVelocity) = 0;

		virtual float GetAngularVelocity(bool Degrees) = 0;
		virtual void  SetAngularVelocity(float AngularVelocity,bool Degrees) = 0;

		//Ended Main Interface Functions//

	public:

	protected:

	protected:

	private:

	private:


	};
}