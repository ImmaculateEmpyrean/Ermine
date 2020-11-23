#pragma once
#include<iostream>
#include<vector>
#include<string>

namespace Ermine
{
	enum class SubscriptionHealth : unsigned int
	{
		WantToTerminate = 0,
		Subscribed = 1
	};
}