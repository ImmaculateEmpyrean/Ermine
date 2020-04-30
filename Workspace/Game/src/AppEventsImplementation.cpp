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