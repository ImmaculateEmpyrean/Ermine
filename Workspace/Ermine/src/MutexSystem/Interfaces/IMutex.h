#pragma once
#include<iostream>
#include<vector>
#include<string>

#include<thread>
#include<mutex>

#include "../MutexManagerLevelConfiguration.h"
#include "MutexSystem/MutexGaurd.h"

/*
	This is An Interface Which Is To Be Implemented By All Classes In Ermine Carrying A Mutex.. At The Time Of Writing.. Mutex Manager Only Senses A Mutex By Using This Interface
*/
namespace Ermine
{
	class IMutex
	{
	public:
		virtual std::unique_lock<std::recursive_mutex> GetUniqueLock() = 0;
		virtual Ermine::MutexLevel GetMutexLevel() = 0;

		virtual Ermine::MutexGaurd GetErmineMutexGaurd() = 0;
	};
}