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

namespace Ermine
{
	void PrintCount();
	int GetCount();
	void ResetCount();

	int GetScreenHeight();
	int GetScreenWidth();

	float NormalizationFunction(float x, float LowestValueInData, float HighestValueInData, float LowestValueInNewRange = -1.0f, float HighestValueInNewRange = 1.0f);
}
