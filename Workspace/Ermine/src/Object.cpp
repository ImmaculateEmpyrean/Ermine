#include "stdafx.h"
#include "Object.h"

namespace Ermine
{
	Object::Object()
	{
		//Generate And Bind Said Generated Object
		GeneratedObjPtr = std::shared_ptr<Ermine::GeneratedObject>(new GeneratedObject());
		GeneratedObjPtr->BindObject(this);
	}

	Object::Object(const Object& rhs)
	{
		auto ForeignLock = rhs.GeneratedObjPtr->GetObjectMutex();
		
		//Generate And Bind Said Generated Object
		GeneratedObjPtr = std::shared_ptr<Ermine::GeneratedObject>(new GeneratedObject());

		auto Lock = GetObjectMutex();

		GeneratedObjPtr->BindObject(this);

		auto Flags = rhs.GeneratedObjPtr->AllFlagsOfRecievingEvents();

		unsigned int FlagCounter = 0;
		for (auto i : Flags)
			GeneratedObjPtr->RecieveEvents(i, Ermine::EventType(FlagCounter++));

	}
	Object& Object::operator=(const Object& rhs)
	{
		auto ForeignLock = rhs.GeneratedObjPtr->GetObjectMutex();

		GeneratedObjPtr = std::shared_ptr<Ermine::GeneratedObject>(new GeneratedObject());

		auto Lock = GetObjectMutex();

		GeneratedObjPtr->BindObject(this);

		auto Flags = rhs.GeneratedObjPtr->AllFlagsOfRecievingEvents();

		unsigned int FlagCounter = 0;
		for (auto i : Flags)
			GeneratedObjPtr->RecieveEvents(i, Ermine::EventType(FlagCounter++));

		return *this;
	}
	Object::Object(Object&& rhs)
	{
		auto ForeignLock = rhs.GeneratedObjPtr->GetObjectMutex();
		
		rhs.GeneratedObjPtr->UnBindObject();
		rhs.GeneratedObjPtr->BindObject(this);


	}
	Object& Object::operator=(Object&& rhs)
	{
		auto ForeignLock = rhs.GeneratedObjPtr->GetObjectMutex();

		rhs.GeneratedObjPtr->UnBindObject();
		rhs.GeneratedObjPtr->BindObject(this);

		return *this;
	}

	Object::~Object()
	{
		auto Lock = GetObjectMutex();

		GeneratedObjPtr->SetObjectHealth(Ermine::ObjectStatus::StatusMarkedForDeletion);
		GeneratedObjPtr->UnBindObject();
	}

	void Object::AssignPushEventsToFunction(std::function<void(Ermine::Event*)> PushToFunction)
	{
		auto Lock = GetObjectMutex();

		if (PushToFunction != nullptr)
		{
			this->PushToFunction = PushToFunction;
			this->PushToFunc = true;
		}
	}

	void Object::DestroyPushEventsToFunction()
	{
		auto Lock = GetObjectMutex();

		this->PushToFunc = false;
		this->PushToFunction = nullptr;
	}

	bool Object::QueryPushEventsToFunctionFlag()
	{
		auto Lock = GetObjectMutex();
		return PushToFunc;
	}

	std::function<void(Ermine::Event*)> Object::GetPushToFunction()
	{
		auto Lock = GetObjectMutex();
		
		assert(PushToFunction != nullptr);
		return PushToFunction;
	}

	void Object::DefaultEventHandler()
	{
		STDOUTDefaultLog_Info("Default Handler Of Event Invoked In Object.. Guess You ARe Unnecessarily Wasting Some Calls..");
	}
}