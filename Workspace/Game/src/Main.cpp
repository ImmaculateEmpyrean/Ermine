#include "stdafx.h"
#include "ExchangeStructs/ExchangeAppEvents.h"

#include "../../Ermine/src/App.h"

Ermine::App* HApp = nullptr;

void DLL _cdecl StoreAppHandle(Ermine::App* Handle)
{
	HApp = Handle;
}


BOOL WINAPI DllMain(_In_ HINSTANCE hinstDLL,_In_ DWORD fdwReason,_In_ LPVOID lpvReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		//The DLL Is Clearly Attaching We May Use This Chance Initialize Stuff..
		std::vector<std::pair<std::string, CreateLogFile>> Configuration;
		Configuration.emplace_back(std::make_pair<std::string, CreateLogFile>("Game", CreateLogFile::CreateLogFile));
		Log::Init(Configuration);

		Log_Trace("Game", "Game DLL Attach Invoked Simply Tracing It.. :>");
	}
	
	return TRUE;
}

void DLL __cdecl GetAppFromGame()
{

}


void DLL __cdecl TestFunc()
{
	
	FileDefaultLog_Error("Han");

	std::cout << "Hello World" << std::endl;
}

void DLL _cdecl OnStart()
{
	STDOUTDefaultLog_Info("Started On Start Function From DLL");

	//auto app = Ermine::App::Get();

	STDOUTDefaultLog_Info(HApp->Name);
}