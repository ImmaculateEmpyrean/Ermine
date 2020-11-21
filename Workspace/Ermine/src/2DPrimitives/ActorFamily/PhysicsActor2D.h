#pragma once
#include<iostream>
#include<vector>
#include<string>

#include<mutex>
#include "MutexSystem/Interfaces/IMutex.h"

#include "ImageBase.h"
#include "Physics/Physics.h"
#include "Physics/PhysicsComponent2D.h"

#include "Interfaces/IMovableActor.h"

/*
	Physics Actor2D Extends ImageBase By Implementing The Physics Component In It..
*/

namespace Ermine
{
	class PhysicsActor:public Ermine::ImageBase, public PhysicsComponent2D,public Ermine::IMovableActor,public  Ermine::IMutex
	{
	public:
#pragma region Constructors  
		//Physics Actor Cannot Be Constructed Defaultly As imageBase Cant Be Constructed Defaultly..
		PhysicsActor() = delete;

		//This is The Most Basic Of The Constructor.. It Does Not hold Any data to assist us in the construction Of The Physics Object.. Instead It Is Going To use The Engine Defaults
		PhysicsActor(std::shared_ptr<Ermine::Sprite> Spr);

		//This Constructor Is To Be Used Most of The Time Must Give The Actor With A Physics COmponent..
		PhysicsActor(std::shared_ptr<Ermine::Sprite> Spr, PhysicsComponent2D Phys);

		//Virtual Destructor For The Children Down The Line..
		virtual ~PhysicsActor() override;

		//Copy And Move Constructors As Well As Operators Are To Be Overrided So As To Account For The Mutex Inside This Class
	public:
		//Will Implement These In The Future When Copy For Physics Component2D Can Be Implemented..
		PhysicsActor(const PhysicsActor& rhs) = delete;
		PhysicsActor& operator=(const PhysicsActor& rhs) = delete;

		PhysicsActor(PhysicsActor&& rhs);
		PhysicsActor& operator=(PhysicsActor&& rhs);
#pragma endregion


#pragma region IMovableActorOverrides
		/*Start Overriding Movable Actor Functions*/
		virtual glm::vec2 GetActorPosition() override;
		virtual void SetActorPosition(glm::vec2 ActorPosition) override;

		virtual glm::vec2 GetActorVelocity() override;
		virtual void SetActorVelocity(glm::vec2 ActorVelocity) override;
		
		virtual float GetAngularVelocity(bool Degrees = true) override;
		virtual void  SetAngularVelocity(float AngularVelocity, bool Degrees) override;
		/*Ended Overriding Movable Actor Functions*/
#pragma endregion
#pragma region IMutexOverrides
		//Start IMutex Overrides//
		virtual Ermine::MutexLevel GetMutexLevel() override { return Ermine::MutexLevel::PhysicsActor; }
		virtual Ermine::MutexGaurd GetErmineMutexGaurd() { return std::move(MutexGaurd(this, Ermine::MutexLevel::PhysicsActor)); };
		//Ended IMutex Overrides//
#pragma endregion

	public:
		//This Function Returns The Screen Location Of The Object In Question
		virtual glm::vec2 GetScreenLocation() override;

		void SetVelocity(glm::vec2 Velocity);
		void SetAngularVelocity(float Velocity);

		virtual std::vector<float> CalculateModelSpaceVertexes(); //This Used To Be An Override.. Instead Remove This Function In Refactor.. Rendering Is No Longer Handled By The Actor Or Its Thread..

		//This Function Has To Be Overriden In all Children Do Not Forget Otherwise One Child May Be Thought Of As The Other..
		virtual Ermine::ActorFamilyIdentifier GetActorFamilyIdentifier() override { return ActorFamilyIdentifier::PhysicsActor2D; }

	public:

	protected:

	protected:

	private:
#pragma region Helpers
		void HelperMove(PhysicsActor&& rhs);
#pragma endregion

	private:


	};
}