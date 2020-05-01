#include"stdafx.h"

#include "ExchangeStructs/ExchangeAppEvents.h"

void AppOnAttach()
{
	STDOUTDefaultLog_Error("On Attach Function Called From DLL");
}

void AppOnTick()
{

}

void AppOnDetach()
{

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
