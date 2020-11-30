#include "stdafx.h"
#include "FixtureColorStruct.h"

#include "RandomNumberGenerator.h"

void Ermine::FixtureColorStruct::Randomize()
{
	Color.x = RandomNumber::GetNumberInRange(0, 256);
	Color.y = RandomNumber::GetNumberInRange(0, 256);
	Color.z = RandomNumber::GetNumberInRange(0, 256);
	Color.z = 128.0f;
}
