#include "stdafx.h"
#include "EntryPoint.h"

#include "EventSystem/EventBroadcastStation.h"

#include "EngineResourceHandlers/GlobalTextureCache.h"
#include "EngineResourceHandlers/Manifest.h"

#include "Log/Log.h"

#include "Core.h"

int main(int argc, char* argv[])
{
#ifdef ER_DEBUG_DEVELOP
	//Start Initialize Memory Leak Checking Module..
	int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	flag |= (_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetDbgFlag(flag);
	//Ended Initialize Memory Leak Checking Module..
#endif

	//The App Which The User Sends Over Is The One Which The Engine Executes..
	Ermine::ExecuteApp();
	
	while (!(Ermine::H_App->Quit))
	{
		//The Routine Is What Is Executed Evry Frame..
		Ermine::H_App->AppRoutine();
	}
  
	Ermine::EventBroadcastStation::DestroyStation();
	Ermine::GlobalTextureCache::ShutDownGlobalTextureCache();
	
#ifdef ThisDoesNotWorkForNow..
	Ermine::EditorDefaultStrings::DestroyContext();
#endif

#ifdef ER_DEBUG_DEVELOP
	//Start Print Detected Memory Leaks To File..
	FILE* pFile;
	freopen_s(&pFile, "MemoryLeakLog.txt", "w", stdout);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
	_CrtDumpMemoryLeaks(); fclose(pFile);
	//Ended Print Detected Memory Leaks To File..
#endif
}