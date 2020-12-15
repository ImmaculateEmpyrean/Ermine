#pragma once
#include<iostream>
#include<vector>
#include<string>

#include "glm.hpp"

#include "2DPrimitives/Constructs/MovableObject.h"

#include "Interfaces/IMovableActor.h"
#include "ShapeBase.h"

namespace Ermine
{
	class Actor2DShape: public MovableObject, public ShapeBase , public IMovableActor
	{
	public:
#pragma region Constructors

		//Well You Can Totally Have An Empty Shape.. Just Not An Default One..
		Actor2DShape();

		//This Is A Slightly Better Constructor As It Gives You Control Over Where The Image Is Drawn
		Actor2DShape(glm::vec2 SpawnPosition);

		//Probably Use This More Often..
		Actor2DShape(std::vector<Ermine::VertexTextured> Points);

		//Guess This Is The Best For Now..
		Actor2DShape(glm::vec2 SpawnPosition,std::vector<Ermine::VertexTextured> Points);

		//This Is The All Inclusive Constructor..
		Actor2DShape(glm::vec2 SpawnPosition, std::vector<Ermine::VertexTextured> Points, std::shared_ptr<Ermine::Sprite> ActorSprite);

		//Start Copy And Move Constructor Are Custom Implemented.. For Now Just Because..
		Actor2DShape(Actor2DShape& rhs);
		Actor2DShape& operator=(Actor2DShape& rhs);

		Actor2DShape(Actor2DShape&& rhs);
		Actor2DShape& operator=(Actor2DShape&& rhs);
		//Ended Copy And Move Constructor Are Custom Implemented.. For Now Just Because..

		//The Destructor Has To Be Implemented Probably
		virtual ~Actor2DShape();

	public:
		//Generate Function..
		std::shared_ptr<Ermine::Actor2DShape> Generate(glm::vec2 SpawnLocation, std::vector<VertexTextured> Vertices, std::shared_ptr<Ermine::Sprite> Spr);
#pragma endregion

#pragma region Actor2DBaseOverrides
	public:
		virtual glm::mat4 GetModelMatrix() override;
#pragma endregion

	public:
#pragma region Actor2DBaseImplementation
		virtual glm::vec2 GetScreenLocation() { return GetActorPosition(); };
#pragma endregion

#pragma region IMovableActorImplementation
		virtual glm::vec2 GetActorPosition() override;
		virtual void SetActorPosition(glm::vec2 ActorPosition)override;

		virtual glm::vec2 GetActorVelocity() override;
		virtual void SetActorVelocity(glm::vec2 ActorVelocity) override;

		virtual float GetAngularVelocity(bool Degrees) override;
		virtual void  SetAngularVelocity(float AngularVelocity, bool Degrees) override;	 
#pragma endregion
	public:

	protected:

	protected:

	private:
		//Use This To Get The Actor Updated..
		virtual void OnUpdateTickEventRecieved() override;

	private:
		
	};
}