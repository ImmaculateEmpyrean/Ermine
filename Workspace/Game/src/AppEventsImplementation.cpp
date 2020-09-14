#include"stdafx.h"

#include "ExchangeStructs/ExchangeAppEvents.h"
#include "../Ermine/src/Physics/PhysicsWorldInitializationStruct.h"

void AppOnAttach()
{
	STDOUTDefaultLog_Error("On Attach Function Called From DLL");
}

void AppOnTick()
{
	//STDOUTDefaultLog_Error("App On Tick Called From DLL");
}

void AppOnDetach()
{
	STDOUTDefaultLog_Error("On Detach Function Called From DLL");
}

ExchangeAppEvents GetAppEventsStruct()
{
	static ExchangeAppEvents Struct;
	Struct.OnAttach = &AppOnAttach;
	Struct.OnDetach = &AppOnDetach;
	Struct.OnTick = &AppOnTick;

	return Struct;
}

std::string GetGameNameString()
{
	return "Ermine Engine";
}

std::pair<int, int> DLL GetGameWindowDiamensions()
{
	return std::make_pair<int, int>(1000, 1000);
}

Ermine::PhysicsWorldInitializationStruct DLL GetPhysicsWorldInitializationStruct()
{
	Ermine::PhysicsWorldInitializationStruct Phy;
	Phy.Gravity = glm::vec2(0.0f, -1.0f);

	return Phy;
}