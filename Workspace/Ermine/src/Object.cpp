#include "stdafx.h"
#include "Object.h"

//Start Static Declarations//
std::atomic<long> Ermine::Object::Counter = 0;
//Ended Static Declarations//


#pragma region Constructors
Ermine::Object::Object()
{
	std::unique_lock<std::recursive_mutex> Gaurd1(ObjectMutex);
	long LC = Counter.fetch_add(1); //I am not entirely sure that the add is completed before mutex i released.. sure enough but not absolutely sure..
	UniqueIdentifier = std::to_string(LC);
}

Ermine::Object::Object(Object& rhs)
{
	std::unique_lock<std::recursive_mutex> Gaurd1(ObjectMutex);
	std::unique_lock<std::recursive_mutex> Gaurd2(rhs.ObjectMutex);

	long LC = Counter.fetch_add(1); //I am not entirely sure that the add is completed before mutex i released.. sure enough but not absolutely sure..
	UniqueIdentifier = std::to_string(LC);

	ObjectHealth = rhs.ObjectHealth;
}
Ermine::Object& Ermine::Object::operator=(Object& rhs)
{
	std::unique_lock<std::recursive_mutex> Gaurd1(ObjectMutex);
	std::unique_lock<std::recursive_mutex> Gaurd2(rhs.ObjectMutex);

	long LC = Counter.fetch_add(1); //I am not entirely sure that the add is completed before mutex i released.. sure enough but not absolutely sure..
	UniqueIdentifier = std::to_string(LC);

	ObjectHealth = rhs.ObjectHealth;

	return *this;
}

Ermine::Object::Object(Object&& rhs)
{
	std::unique_lock<std::recursive_mutex> Gaurd1(ObjectMutex);
	std::unique_lock<std::recursive_mutex> Gaurd2(rhs.ObjectMutex);

	UniqueIdentifier = rhs.UniqueIdentifier;
	rhs.UniqueIdentifier = std::string("Error No Unique Identifier Detected..");

	ObjectHealth = rhs.ObjectHealth;
	rhs.ObjectHealth = Ermine::ObjectStatus::StatusMarkedForDeletion;
}
Ermine::Object& Ermine::Object::operator=(Object&& rhs)
{
	std::unique_lock<std::recursive_mutex> Gaurd1(ObjectMutex);
	std::unique_lock<std::recursive_mutex> Gaurd2(rhs.ObjectMutex);

	UniqueIdentifier = rhs.UniqueIdentifier;
	rhs.UniqueIdentifier = std::string("Error No Unique Identifier Detected..");

	ObjectHealth = rhs.ObjectHealth;
	rhs.ObjectHealth = Ermine::ObjectStatus::StatusMarkedForDeletion;

	return *this;
}
#pragma endregion

