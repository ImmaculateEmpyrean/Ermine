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


	public:

	public:

	protected:

	protected:

	private:

	private:


	};
}