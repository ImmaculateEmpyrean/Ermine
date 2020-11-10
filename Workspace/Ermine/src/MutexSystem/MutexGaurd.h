#pragma once
#include<iostream>
#include<vector>
#include<string>

#include "Object.h"

#include "MutexManager.h"
#include "MutexCasket.h"
#include "MutexManagerLevelConfiguration.h"

namespace Ermine
{
class MutexGaurd;
#define MUTEXGAURD(MutexLeveL) MutexGaurd MutexGaurdObj = MutexGaurd(this,MutexLeveL);

	class MutexGaurd
	{
	public:
		//No Need For An Empty Gaurd Atleast For Now..
		MutexGaurd() = delete;

		//This Is The Standrad Mutex Constructor See That You Use It Often..
		MutexGaurd(Ermine::Object* Obj, Ermine::MutexLevel Level);

		//The Destructor Is Used To Unlock If Its Not Already Unlocked
		~MutexGaurd();

		//The Mutexes Inside Cannot Be COpied Hence This Function Is Non Copiable..
		MutexGaurd(const MutexGaurd& rhs) = delete;
		MutexGaurd& operator=(const MutexGaurd& rhs) = delete;

		MutexGaurd(MutexGaurd&& rhs);
		MutexGaurd& operator=(MutexGaurd&& rhs);
	public:
		void Unlock();
		void Lock();

		MutexState GetMutexState();
	protected:

	protected:

	private:

	private:
		Ermine::Object* ObjectRef = nullptr;
		Ermine::MutexLevel Level = Ermine::MutexLevel::Empty;

		bool Unlocked = false;
	};
}