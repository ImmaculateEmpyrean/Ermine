#pragma once
#include<iostream>
#include<vector>
#include<string>

#include "Physics/Physics.h"

namespace Ermine
{
	class JointBase
	{
	public:
		//No Point In Having A Default Revolute Joint.. The Joint Must Hold Bodies..
		JointBase() = delete;

		//This Is Something Like The Default Constructor In This Case Use It Most Of The Time
		JointBase(b2Body* BodyA, b2Body* BodyB, bool CollisionEnableBetweenTheBoduesWhichShareTheJoint);

		//The Destructor Is Very Vital As It Must Inform Universum To Delete A Jointy At The Appropriate Time.. Its Made Virtual Because Why NOT..
		virtual ~JointBase();

	public:
		//Donot Make Duplicates In The Physics World
		JointBase(const JointBase& rhs) = delete;  
		JointBase operator=(const JointBase& rhs) = delete;

		//Use Move Primarily In The Place Of Copy.. This Might Not Be Too Much Of A Problem Since PhysicsComponent2D Also Does Not Like To Copy..
		JointBase(JointBase&& rhs);
		JointBase& operator=(JointBase&& rhs);

		//This Class HAs The Ability To Be Implicitly Converted To A b2Joint* Try Not To Misuse This Too Much..
		operator b2Joint*();

		//This Identifier Is Unique To This JointBase Instance It Is Generated On Instantiation And As JointBase Is Uncopiable It Hopefully Is Accessible As An Identifier Of Sorts..
		unsigned int GetUniqueIdentifier();

	public:

	protected:

	protected:

	private:

	private:
		//This Variable Is Used To Assign The Identifier To Each Instance Of The Class.. 
		static unsigned int InstanceCounter;

		//0 Means Nothing.. The GetUniqueIdentifier() Must Never Return 0..
		unsigned int UniqueIdentifier = 0;
		b2Joint* JointHeld;
	};
}