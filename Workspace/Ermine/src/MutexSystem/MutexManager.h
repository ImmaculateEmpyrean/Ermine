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
		void AcquireMutex(Ermine::Object* Obj, MutexLevel WhichMutex);
		void ReleaseMutex(Ermine::Object* Obj, MutexLevel WhichMutex);

	protected:

	protected:

	private:
		std::unique_lock<std::recursive_mutex> GetMutex(std::string ObjectName, unsigned int LevelNumber);

	private:
		//This Is The Singleton Pointer..
		static thread_local MutexManager* Manager;
		static thread_local std::once_flag InitializationFlag;

		std::unordered_map<std::string, std::vector<Ermine::MutexCasket>> StorageBuffer;

		std::unordered_map<std::string, std::unordered_map<unsigned int, std::recursive_mutex>> MutexStorage;
	};
}