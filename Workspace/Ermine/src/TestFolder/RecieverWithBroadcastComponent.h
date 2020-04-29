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
	{
		Ermine::RecieverComponent RecieverComp(GenCallableFromMethod(&RecieverWithBroadcastComponent::TestFunction), Flag, Ermine::EventType::ConcreteEvent);
	}

	std::atomic<bool> Flag;

	void TestFunction(Ermine::Event* EveObj)
	{
		std::cout << "Called Test Function" << std::endl;
	}


};