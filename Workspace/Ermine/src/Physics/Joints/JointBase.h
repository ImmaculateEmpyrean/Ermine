#pragma once
#include<iostream>
#include<vector>
#include<string>

#include "Physics/Physics.h"
#include "JointHealthEnum.h"
#include "EJointType.h"

namespace Ermine
{
	class JointBase
	{
	public:
		//All Joints Must Atleast Posess A Name Otherwise Destruction And Getting Will Become Complicated..
		JointBase() = delete;

		//This Is Something Like The Default Constructor In This Case Use It Most Of The Time
		JointBase(std::string JointName, b2Body* BodyA, b2Body* BodyB);

		//The Destructor Is Very Vital As It Must Inform Universum To Delete A Jointy At The Appropriate Time.. Its Made Virtual Because Why NOT..
		virtual ~JointBase();

	public:
		//Donot Make Duplicates In The Physics World
		JointBase(const JointBase& rhs) = delete;
		JointBase operator=(const JointBase& rhs) = delete;

		//Use Move Primarily In The Place Of Copy.. This Might Not Be Too Much Of A Problem Since PhysicsComponent2D Also Does Not Like To Copy..
		JointBase(JointBase&& rhs);
		JointBase& operator=(JointBase&& rhs);

		//On The Fly Check If The JointIs Enabled..
		bool IsJointEnbaled() { return JointHandle->IsEnabled(); };

		//Anyone Can Request For The Underlying Handle
		b2Joint* GetHandle() { return JointHandle; }

		//Get The Concrete Type Of The Joint In Question..
		virtual Ermine::EJointType GetType() = 0;

		//Get Collide Connected Flag From The Joint..
		bool GetCollideConnected() { return JointHandle->GetCollideConnected(); }

		//Shift Origin..
		void ShiftOrigin(glm::vec2 NewOrigin);

		//You Are Only Allowed To See The Health Of Any Said Joint.. Handling Is Done Implicitly..
		Ermine::JointHealthEnum GetHealth() { return Health; }
		
		//Use This To Destroy Said Joint..
		void DestroyJoint();

		//This Identifier Is Unique To This JointBase Instance It Is Generated On Instantiation And As JointBase Is Uncopiable It Hopefully Is Accessible As An Identifier Of Sorts..
		unsigned int GetUniqueIdentifier();

		//Get The Bodies In Question.. Note Joint Base Does Not Own The Body Pointers.. Be VEry Careful Donot Choose To Delete Them
		b2Body* GetBodyA() { return BodyA; }
		b2Body* GetBodyB() { return BodyB; }

		//Start Functions To Get The Anchor Locations
		glm::vec2 GetBodyAWorldAnchorLocation();
		glm::vec2 GetBodyBWorldAnchorLocation();

		//Ended Functions To Get The Anchor Locations

		//Set The User Data Assigned To The Joint If Needed..
		void SetUserData(void* Data);
		
		//Get The User Data Assigned To The Joint If Needed..
		void* GetUserData();

		std::string GetName() { return Name; };
		void SetName(std::string Name) { this->Name = Name; };

	public:
		

	protected:

	protected:
		b2Joint* JointHandle = nullptr;

	private:

	private:
		//This Variable Is Used To Assign The Identifier To Each Instance Of The Class.. 
		static std::atomic<unsigned int> InstanceCounter;

		//This Is The Name With Which This Joint Is Referred To..
		std::string Name = "Error";

		//This Is The State In Which The JointBase Is.. If Its Marked For Deletion.. You Probably Shouldnt Use This
		Ermine::JointHealthEnum Health = Ermine::JointHealthEnum::StatusOk;

		//0 Means Nothing.. The GetUniqueIdentifier() Must Never Return 0..
		unsigned int UniqueIdentifier = 0;
		
		b2Body* BodyA = nullptr;
		b2Body* BodyB = nullptr;
	};
}