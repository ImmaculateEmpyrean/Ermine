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

	Object::Object(std::string Name)
	{
		//Set The Object Name Properly
		Object_Name = Name;

		//Generate And Bind Said Generated Object
		GeneratedObjPtr = std::shared_ptr<Ermine::GeneratedObject>(new GeneratedObject());
		GeneratedObjPtr->BindObject(this);
	}

	Object::Object(const Object& rhs)
	{
		auto ForeignLock = rhs.GeneratedObjPtr->GetObjectMutex();
		
		//Set The Name Taking It From The rhs Object..
		Object_Name = rhs.Object_Name;

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

		//Set The Name Taking It From The rhs Object..
		Object_Name = rhs.Object_Name;

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
		auto Lock = GetObjectMutex();

		//Set The Name Taking It From The rhs Object..
		Object_Name = std::move(rhs.Object_Name);

		ObjectHealth = rhs.ObjectHealth;
		GeneratedObjPtr = rhs.GeneratedObjPtr;

		PushToFunction = nullptr;
		PushToFunc = false;

		rhs.GeneratedObjPtr = nullptr;
		rhs.ObjectHealth = Ermine::ObjectStatus::StatusMarkedForDeletion;
	}
	Object& Object::operator=(Object&& rhs)
	{
		auto ForeignLock = rhs.GeneratedObjPtr->GetObjectMutex();
		auto Lock = GetObjectMutex();

		//Set The Name Taking It From The rhs Object..
		Object_Name = std::move(rhs.Object_Name);

		ObjectHealth = rhs.ObjectHealth;
		GeneratedObjPtr = rhs.GeneratedObjPtr;

		PushToFunction = nullptr;
		PushToFunc = false;

		rhs.GeneratedObjPtr = nullptr;
		rhs.ObjectHealth = Ermine::ObjectStatus::StatusMarkedForDeletion;

		return *this;
	}

	Object::~Object()
	{
		auto Lock = GetObjectMutex();

		GeneratedObjPtr->SetObjectHealth(Ermine::ObjectStatus::StatusMarkedForDeletion);
		GeneratedObjPtr->UnBindObject();
	}

#pragma region ObjectHealth
	Ermine::ObjectStatus Object::GetObjectHealth()
	{
		auto Lock = GetObjectMutex();
		return ObjectHealth;
	}

	void Object::SetObjectHealth(Ermine::ObjectStatus Status)
	{
		auto Lock = GetObjectMutex();
		ObjectHealth = Status;
	}

	void Object::MarkObjectForDeletion()
	{
		auto Lock = GetObjectMutex();
		ObjectHealth = Ermine::ObjectStatus::StatusMarkedForDeletion;
	}

	void Object::RestoreObjectHealth()
	{
		auto Lock = GetObjectMutex();
		ObjectHealth = Ermine::ObjectStatus::StatusOk;
	}
#pragma endregion

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

#pragma region InterfaceToInteractWithGeneratedObject
	bool Object::IsRecievingConcreteEvents()
	{
		auto Lock = GetObjectMutex();
		return GeneratedObjPtr->IsRecievingConcreteEvents();
	}

	bool Object::IsRecievingKeyCallbackEvents()
	{
		auto Lock = GetObjectMutex();
		return GeneratedObjPtr->IsRecievingKeyCallbackEvents();
	}

	bool Object::IsRecievingCharacterKeyCallbackEvents()
	{
		auto Lock = GetObjectMutex();
		return GeneratedObjPtr->IsRecievingCharacterKeyCallbackEvents();
	}

	bool Object::IsRecievingCursorPositionUpdateEvents()
	{
		auto Lock = GetObjectMutex();
		return GeneratedObjPtr->IsRecievingCursorPositionUpdateEvents();
	}

	bool Object::IsRecievingMouseButtonCallbackEvents()
	{
		auto Lock = GetObjectMutex();
		return GeneratedObjPtr->IsRecievingMouseButtonCallbackEvents();
	}

	bool Object::IsRecievingScrollUpdateEvents()
	{
		auto Lock = GetObjectMutex();
		return GeneratedObjPtr->IsRecievingScrollUpdateEvents();
	}

	bool Object::IsRecievingTileSelectedCallbackEvents()
	{
		auto Lock = GetObjectMutex();
		return GeneratedObjPtr->IsRecievingTileSelectedCallbackEvents();
	}

	bool Object::IsRecievingOnRenderTickEvents()
	{
		auto Lock = GetObjectMutex();
		return GeneratedObjPtr->IsRecievingOnRenderTickEvents();
	}

	bool Object::IsRecievingOnUpdateTickEvents()
	{
		auto Lock = GetObjectMutex();
		return GeneratedObjPtr->IsRecievingOnUpdateTickEvents();
	}

	bool Object::IsRecievingOnBeginEvents()
	{
		auto Lock = GetObjectMutex();
		return GeneratedObjPtr->IsRecievingOnBeginEvents();
	}

	std::vector<bool> Object::AllFlagsOfRecievingEvents()
	{
		auto Lock = GetObjectMutex();
		return GeneratedObjPtr->AllFlagsOfRecievingEvents();
	}

	std::string Object::GetUniqueIdentifier()
	{
		auto Lock = GetObjectMutex();
		return GeneratedObjPtr->GetUniqueIdentifier();
	}

	void Object::RecieveEvents(bool Flag, Ermine::EventType TypeOfEventToRecieve)
	{
		auto Lock = GetObjectMutex();
		GeneratedObjPtr->RecieveEvents(Flag, TypeOfEventToRecieve);
	}
#pragma endregion

	void Object::DefaultEventHandler()
	{
		STDOUTDefaultLog_Info("Default Handler Of Event Invoked In Object.. Guess You ARe Unnecessarily Wasting Some Calls..");
	}
}