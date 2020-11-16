#pragma once
#include<iostream>
#include<vector>
#include<string>

namespace Ermine
{
	enum class ObjectStatus : unsigned int
	{
		StatusMarkedForDeletion = 0,
		StatusOk = 1
	};
}