#pragma once
#include<iostream>
#include<vector>
#include<string>

#include "ImageBase.h"
#include "Physics/Physics.h"
#include "Physics/PhysicsComponent2D.h"

/*
	Physics Actor2D Extends ImageBase By Implementing The Physics Component In It..
*/

namespace Ermine
{
	class PhysicsActor:public Ermine::ImageBase, public PhysicsComponent2D
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

	public:
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