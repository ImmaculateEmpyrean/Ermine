#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<unordered_map>

#include<Object.h>

#include<thread>
#include<mutex>

#include "MutexCasket.h"
#include "MutexManagerLevelConfiguration.h"
#include "MutexState.h"

/*
	This Singleton Class Is Per Thread..  Each Thread Has A Different Singleton Associated With It
*/

namespace Ermine
{
	class MutexManager
	{
	private: 
		//The Object Maybe Constructed Without Parameteters.. Just Not Defaultly..
		MutexManager() = default; 

	public:
		static MutexManager* GetManager();

	public:
		//The Mutex State Says Is It Okay To Carry On After You Have Acquired Your Mutex..
		Ermine::MutexState AcquireMutex(Ermine::Object* Obj, MutexLevel WhichMutex);

		//If The Mutex State Is Not Okay Then.. Say It Is Something Like The Object Is Being Destroyed Or Something Like That.. At Any Rate If A Certain Mutex Is Not Okay.. The Manager May Get Rid Of It At Its Own Voilition
		void ReleaseMutex(Ermine::Object* Obj, MutexLevel WhichMutex, Ermine::MutexState StateOfTheMutex = Ermine::MutexState::Okay);

		
	protected:

	protected:

	private:
		std::unique_lock<std::recursive_mutex> GetMutex(std::string ObjectName, unsigned int LevelNumber);

		Ermine::MutexState GetMutexState(std::string ObjectName);
		void SetMutexState(std::string ObjectName,Ermine::MutexState State);

	private:
		//This Is The Singleton Pointer..
		static MutexManager* Manager; //There IS Only One Manager And It Owns All Mutexes No More One Manager Per Thread Nonesense..
		static std::once_flag InitializationFlag;

		//I think There Is No Need Of This.. Since Mutexes Are Now Stored With The Manager.. But It Maybe Required As A Place To Actually Store All The Lock Gaurds Held By The Objects..
		std::unordered_map<std::string, std::vector<Ermine::MutexCasket>> StorageBuffer;

		std::unordered_map<std::string, std::pair<Ermine::MutexState,std::unordered_map<unsigned int, std::recursive_mutex>>> MutexStorage;//std::unordered_map<unsigned int, std::pair<Ermine::MutexState,std::recursive_mutex>>> MutexStorage;
	};
}