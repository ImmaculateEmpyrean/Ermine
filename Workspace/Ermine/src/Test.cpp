#include "stdafx.h"

#include "EventSystem/EventBroadcastStation.h"

#include "TestFolder/RecieverWithBroadcastComponent.h"
#include "TestFolder/SenderWithSendComponent.h"

#include "Log.h"

#include "LayerSystem/LayerStack.h"
#include "LayerSystem/LayerStackLayer.h"

#include "App.h"

#include "TestFolder/TestInputSystem/InputChecker.h"

#include "EngineResourceHandlers/GlobalTextureCache.h"

void DLL __cdecl TestFunc();

int main(int argc, char* argv[])
{
	//Start Initialize Memory Leak Checking Module..
	int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	flag |= (_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetDbgFlag(flag);
	//Ended Initialize Memory Leak Checking Module..

	auto App = Ermine::App::Get();
	Test::InputChecker InpObj;
	while (!App->Quit)
	{
		App->NextFrame();
	}

	int* n = new int[100];
	Ermine::EventBroadcastStation::DestroyStation();
	Ermine::GlobalTextureCache::ShutDownGlobalTextureCache();

	//Start Print Detected Memory Leaks To File..
	FILE* pFile;
	freopen_s(&pFile, "MemoryLeakLog.txt", "w", stdout);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
	_CrtDumpMemoryLeaks(); fclose(pFile);
	//Ended Print Detected Memory Leaks To File..
}