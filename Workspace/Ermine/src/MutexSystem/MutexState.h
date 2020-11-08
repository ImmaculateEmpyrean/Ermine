#pragma once
#include<iostream>
#include<vector>
#include<string>

#include<mutex>

namespace Ermine
{
	enum class MutexState : unsigned int
	{
		Okay = 1,
		NotOkay = 0
	};
}