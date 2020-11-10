#pragma once
#include<iostream>
#include<vector>
#include<string>

#include<atomic>

namespace Ermine
{
	enum class ObjectStatus: unsigned int
	{
		StatusMarkedForDeletion=0,
		StatusOk=1
	};

	class Object
	{
	public:
		//Object Can Be Constructed Without Much Prior Construction For Now.. It Just Cant Be Constructed Defaultly
		Object();

		//Start Since Object Contains A Mutex Lets Go With Defining Copy And Move Operations..//
		Object(Object& rhs);
		Object& operator=(Object& rhs);

		Object(Object&& rhs);
		Object& operator=(Object&& rhs);
		//Ended Since Object Contains A Mutex Lets Go With Defining Copy And Move Operations..//
	public:
		std::string GetUniqueIdentifier() { return UniqueIdentifier; }
		
	public:
		Ermine::ObjectStatus GetObjectHealth() { return ObjectHealth; }

		void SetObjectHealth(Ermine::ObjectStatus Status) { ObjectHealth = Status; }
		void MarkObjectForDeletion() { ObjectHealth = Ermine::ObjectStatus::StatusMarkedForDeletion; };
		void RestoreObjectHealth() { { ObjectHealth = Ermine::ObjectStatus::StatusOk; }; }

		std::unique_lock<std::recursive_mutex> GetObjectMutex() { return std::move(std::unique_lock<std::recursive_mutex>(ObjectMutex)); };
	protected:

	protected:

	private:
		virtual void Dummy() {}

	private:
		static std::atomic<long> Counter;
		std::string UniqueIdentifier;

		ObjectStatus ObjectHealth = Ermine::ObjectStatus::StatusOk;

		std::recursive_mutex ObjectMutex;
	};
}