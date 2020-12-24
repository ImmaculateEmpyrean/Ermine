#pragma once
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#if defined(ER_DEBUG_DEVELOP) || defined(ER_DEBUG_SHIP)
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
#define DBG_NEW new
#endif


#define BIT(x) (1<<x)

#include "DeltaTime.h"
#include "Level/Scene.h"

namespace Ermine
{
	int GetScreenHeight();
	int GetScreenWidth();

	float NormalizationFunction(float x, float LowestValueInData, float HighestValueInData, float LowestValueInNewRange = -1.0f, float HighestValueInNewRange = 1.0f);
	
	//This Structure Contains Information Abt Delta Time And Is Updated By The App Class..
	extern DeltaTime TimeStep;

	
	//Forward Declaration Of App To Be Able To Hold A Pointer To Said App...
	class App;
	//A Handle To The App Class..
	extern Ermine::App* H_App;

	//This Function Is Used To Get A Handle To The Current App In Question..
	Ermine::App* GetHandleToApp();
}
