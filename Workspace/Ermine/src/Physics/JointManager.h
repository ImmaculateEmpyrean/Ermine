#pragma once
#include<iostream>
#include<vector>
#include<string>

#include<unordered_map>
#include<memory>

#include "JointTicket.h"
#include <box2d/b2_joint.h>

namespace Ermine
{
	class JointManager
	{
	//JointManager Is Being Constructed To Be A Singleton..
	private:
		//I Dont Think Its That Difficult To Construct The Joint Base.. Besides Its Only Constructed Once..
		JointManager();
		
		//Start There Is Absolutely No Need For JointManager Construction//
		JointManager(const JointManager& rhs) = delete;
		JointManager& operator=(const JointManager& rhs) = delete;

		JointManager(JointManager&& rhs) = delete;
		JointManager& operator=(JointManager&& rhs) = delete;
		//Ended There Is Absolutely No Need For JointManager Construction//

	public:

	public:

	protected:

	protected:

	private:

	private:
		std::unordered_map<unsigned int, b2Joint*> JointsBuffer;
	};
}