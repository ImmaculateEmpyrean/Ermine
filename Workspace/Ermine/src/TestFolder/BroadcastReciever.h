#pragma once
#include<iostream>
#include<vector>

#include "EventSystem/EventBroadcastStation.h"

class BroadcastReciever
{
public:
	BroadcastReciever();
private:
	void Event(Ermine::Event& eve);
public:
};