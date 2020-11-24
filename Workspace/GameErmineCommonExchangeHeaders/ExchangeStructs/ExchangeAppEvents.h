#pragma once
#include<iostream>
#include<vector>
#include<string>

#include<functional>

//#define DLL __declspec(dllexport)

//#include "../../Ermine/src/App.h"

namespace Ermine
{
	class PhysicsWorldInitializationStruct;
}

struct DLL ExchangeAppEvents
{
public:
	void (*OnAttach)();
	void (*OnTick)();
	void (*OnDetach)();
};

namespace Ermine
{
	class App;
}
extern std::string DLL GetGameNameString(); //Only Used In Shipping Builds Otherwise Stripped Out By Ermine
extern std::pair<int, int> DLL GetGameWindowDiamensions(); //Used Everyehere.. Maybe Not Used When Building For Android In The Future

extern ExchangeAppEvents DLL GetAppEventsStruct(); //This Must Be Implemented In The Game...

extern Ermine::PhysicsWorldInitializationStruct DLL GetPhysicsWorldInitializationStruct();

void DLL  StoreAppHandle(Ermine::App* Handle);
void DLL OnStart();