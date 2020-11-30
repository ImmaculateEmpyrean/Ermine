#pragma once
#include<iostream>
#include<vector>
#include<string>

#include "FixtureColorStruct.h"

namespace Ermine
{
	struct FixtureUserDataStruct
	{
	public:
		//Start Flags//
		
		//Set This To True And The Renderable Is Use The Color Sored In Debug Trace Color To Draw The Outline On The Screen..
		int UseDebugTraceColor = false;
		//Ended Flags

		//Start Contents//

		FixtureColorStruct DebugTraceColor;
		//Ended Contents//
	};
}