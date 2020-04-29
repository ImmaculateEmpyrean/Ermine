#pragma once
#include<iostream>
#include<vector>
#include<string>

#include<atomic>

#include "EventSystem/EnumEventType.h"
#include "EventSystem/Components/RecieverComponent.h"

class RecieverWithBroadcastComponent
{
public:
	RecieverWithBroadcastComponent()
		:
		Flag(true)
	{
		Ermine::RecieverComponent RecieverComp(GenCallableFromMethod(&RecieverWithBroadcastComponent::TestFunction), Flag, Ermine::EventType::ConcreteEvent);
	}

	std::atomic<bool> Flag = true;

	void TestFunction(Ermine::Event* EveObj)
	{
		auto ptr = (Ermine::ConcreteEvent*)EveObj;
		std::cout << "Called Test Function" << std::endl;
		std::cout << ptr->GetMessageBuffer()<<std::endl;
	}
};