#include "stdafx.h"
#include "MutexGaurd.h"

Ermine::MutexGaurd::MutexGaurd(Ermine::Object* Obj, Ermine::MutexLevel Level)
	:
	ObjectRef(Obj),
	Level(Level)
{
	if (ObjectRef != nullptr)
	{
		if (Level != Ermine::MutexLevel::Empty)
		{
			auto Manager = Ermine::MutexManager::GetManager();
			Manager->AcquireMutex(Obj, Level);
			Unlocked = false;
		}
		else
		{
			STDOUTDefaultLog_Error("Cannot Aqquire Mutex Of An Object Whose Level Is Empty");
			Unlocked = true;
		}
	}
	else
	{
		STDOUTDefaultLog_Error("Cannot Aqquire Mutex Of An Object Which Is Null");
		Unlocked = true;
	}
}

Ermine::MutexGaurd::~MutexGaurd()
{
	if (Unlocked == false)
	{
		if (ObjectRef != nullptr)
		{
			if (Level != Ermine::MutexLevel::Empty)
			{
				auto Manager = Ermine::MutexManager::GetManager();
				Manager->ReleaseMutex(ObjectRef, Level);
			}
			else
			{
				STDOUTDefaultLog_Error("Cannot Release Mutex Of An Object Whose Level Is Empty");
			}
		}
		else
		{
			STDOUTDefaultLog_Error("Cannot Release Mutex Of An Object Which Is Null");
		}
	}
}

Ermine::MutexGaurd::MutexGaurd(MutexGaurd&& rhs)
{
	ObjectRef = rhs.ObjectRef;
	rhs.ObjectRef = nullptr;

	Level = rhs.Level;
	rhs.Level = Ermine::MutexLevel::Empty;

	Unlocked = rhs.Unlocked;
}

Ermine::MutexGaurd& Ermine::MutexGaurd::operator=(MutexGaurd&& rhs)
{
	ObjectRef = rhs.ObjectRef;
	rhs.ObjectRef = nullptr;

	Level = rhs.Level;
	rhs.Level = Ermine::MutexLevel::Empty;

	Unlocked = rhs.Unlocked;

	return *this;
}

void Ermine::MutexGaurd::Unlock()
{
	if (Unlocked == false)
	{
		if (ObjectRef != nullptr)
		{
			if (Level != Ermine::MutexLevel::Empty)
			{
				auto Manager = Ermine::MutexManager::GetManager();
				Manager->ReleaseMutex(ObjectRef, Level);
				Unlocked = true;
			}
			else
			{
				STDOUTDefaultLog_Error("Cannot Release Mutex Of An Object Whose Level Is Empty");
			}
		}
		else
		{
			STDOUTDefaultLog_Error("Cannot Release Mutex Of An Object Which Is Null");
		}
	}
}

void Ermine::MutexGaurd::Lock()
{
	if (Unlocked == true)
	{
		if (ObjectRef != nullptr)
		{
			if (Level != Ermine::MutexLevel::Empty)
			{
				auto Manager = Ermine::MutexManager::GetManager();
				Manager->AcquireMutex(ObjectRef, Level);
				Unlocked = false;
			}
			else
			{
				STDOUTDefaultLog_Error("Cannot Aquire Mutex Of An Object Whose Level Is Empty");
			}
		}
		else
		{
			STDOUTDefaultLog_Error("Cannot Aquire Mutex Of An Object Which Is Null");
		}
	}
}
