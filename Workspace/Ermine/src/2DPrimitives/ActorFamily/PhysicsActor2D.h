#pragma once
#include<iostream>
#include<vector>
#include<string>

#include "ImageBase.h"
#include "Physics/Physics.h"
#include "Physics/PhysicsComponent2D.h"

#include "Interfaces/MovableActor.h"

/*
	Physics Actor2D Extends ImageBase By Implementing The Physics Component In It..
*/

namespace Ermine
{
	class PhysicsActor:public Ermine::ImageBase, public PhysicsComponent2D,public Ermine::MovableActor 
	{
	public:
		//Physics Actor Cannot Be Constructed Defaultly As imageBase Cant Be Constructed Defaultly..
		PhysicsActor() = delete;

		//This is The Most Basic Of The Constructor.. It Does Not hold Any data to assist us in the construction Of The Physics Object.. Instead It Is Going To use The Engine Defaults
		PhysicsActor(std::shared_ptr<Ermine::Sprite> Spr);

		//This Constructor Is To Be Used Most of The Time Must Give The Actor With A Physics COmponent..
		PhysicsActor(std::shared_ptr<Ermine::Sprite> Spr, PhysicsComponent2D Phys);

		//Virtual Destructor For The Children Down The Line..
		virtual ~PhysicsActor() override;

		/*Start Overriding Movable Actor Functions*/
		virtual glm::vec2 GetActorPosition() override;
		virtual void SetActorPosition(glm::vec2 ActorPosition) override;

		virtual glm::vec2 GetActorVelocity() override;
		virtual void SetActorVelocity(glm::vec2 ActorVelocity) override;
		
		virtual float GetAngularVelocity(bool Degrees = true) override;
		virtual void  SetAngularVelocity(float AngularVelocity, bool Degrees) override;
		/*Ended Overriding Movable Actor Functions*/

	public:
		//This Function Returns The Screen Location Of The Object In Question
		virtual glm::vec2 GetScreenLocation() override;

		void SetVelocity(glm::vec2 Velocity);
		void SetAngularVelocity(float Velocity);

		virtual std::vector<float> CalculateModelSpaceVertexes() override;

		//This Function Has To Be Overriden In all Children Do Not Forget Otherwise One Child May Be Thought Of As The Other..
		virtual Ermine::ActorFamilyIdentifier GetActorFamilyIdentifier() override { return ActorFamilyIdentifier::PhysicsActor2D; }

	public:

	protected:

	protected:

	private:
		
	private:


	};
}