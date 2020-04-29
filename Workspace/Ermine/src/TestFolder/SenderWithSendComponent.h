#pragma once
#include<iostream>
#include<string>
#include<vector>

#include "EventSystem/Components/BroadcastComponent.h"

class SenderA
{
public:
	SenderA(std::string Message)
		:
		Message(Message)
	{}
private:
	std::string Message;

public:
	void Broadcast()
	{
		std::unique_ptr<Ermine::ConcreteEvent> EventObj = Ermine::ConcreteEvent::GenerateEvent(Message);
		
		Ermine::BroadcastComponent Component;
		Component.BroadcastEvent(std::move(EventObj));
	}
};