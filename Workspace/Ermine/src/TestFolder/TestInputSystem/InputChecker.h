#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<atomic>

#include "EventSystem/EnumEventType.h"
#include "EventSystem/Event.h"
#include "EventSystem/Components/RecieverComponent.h"

namespace Test
{
	class InputChecker
	{
	public:
		InputChecker();
		void CallBackFunc(Ermine::Event* eveobj);

	private:
		std::atomic<bool> Flag = true;
	};
}