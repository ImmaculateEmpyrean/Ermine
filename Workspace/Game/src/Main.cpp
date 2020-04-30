#include "stdafx.h"

BOOL WINAPI DllMain(_In_ HINSTANCE hinstDLL,_In_ DWORD fdwReason,_In_ LPVOID lpvReserved)
{
	return TRUE;
}
void DLL __cdecl TestFunc()
{
	std::vector<std::pair<std::string, CreateLogFile>> Configuration;
	Configuration.emplace_back(std::make_pair<std::string, CreateLogFile>("Lother", CreateLogFile::CreateLogFile));
	Log::Init(Configuration);

	//Log::GetDefaultSTDOUTLogger()->trace("This IS Sigmar");
	STDOUTDefaultLog_Error("I Can Print Here From The DLL");
	FileDefaultLog_Error("Han");

	std::cout << "Hello World" << std::endl;
}