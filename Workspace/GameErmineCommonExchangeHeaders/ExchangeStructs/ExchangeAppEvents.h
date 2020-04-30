#pragma once
#include<iostream>
#include<vector>
#include<string>

#include<functional>

//#define DLL __declspec(dllexport)

struct DLL ExchangeAppEvents
{
public:
	void (*OnAttach)();
	void (*OnTick)();
	void (*OnDetach)();
};

extern ExchangeAppEvents DLL GetAppEventsStruct(); //This Must Be Implemented In The Game...