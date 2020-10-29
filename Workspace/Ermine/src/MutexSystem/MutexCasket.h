#pragma once
#include<iostream>
#include<vector>
#include<string>

#include<optional>

#include<mutex>
#include "MutexManagerLevelConfiguration.h"

namespace Ermine
{
	class MutexCasket
	{
	public:
		//No Point In Having An Empty Casket.. - More Importantly Donot Have A Mutex You Dont Know Which Level It Belongs To..
		MutexCasket() = delete;

		//Only Legal Way For Now To Construct This Casket
		MutexCasket(std::unique_lock<std::recursive_mutex> Mutex, Ermine::MutexLevel LevelNumber);

		MutexCasket(const MutexCasket& rhs) = delete; //Since Mutexes Cannot Be Copied
		MutexCasket& operator=(const MutexCasket& rhs) = delete; //Since Mutexes Cannot Be Copied

		MutexCasket(MutexCasket&& rhs); //Since Mutexes Cannot Be Copied
		MutexCasket& operator=(MutexCasket&& rhs); //Since Mutexes Cannot Be Copied

		~MutexCasket() = default; //Since No Special Destruction Is Needed For Now

		//Get The Mutex Contained In The Casket
		std::optional<std::unique_lock<std::recursive_mutex>> GetMutex();

		//Get The Mutex Level Contained In The Casket
		std::optional<Ermine::MutexLevel> GetLevelNumber();

		//This Gives You A Level Number Yes.. But Please Dont Use It If Not Necessary.. U Dont Know Anything That HAppens After A Mutex Is Removed..
		Ermine::MutexLevel ForceGetLevelNumber();

	private:
		//A Helper To Execute Move Operation
		void MoveHelper(MutexCasket&& rhs);

	private:
		std::unique_lock<std::recursive_mutex> Mutex;
		Ermine::MutexLevel LevelNumber;

		bool ContainsMutex = false; //Flag To Check If Mutex Is Available Inside The Structure..
	};
}