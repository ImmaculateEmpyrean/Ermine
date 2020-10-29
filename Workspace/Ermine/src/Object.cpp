#include "stdafx.h"
#include "Object.h"

//Start Static Declarations//
int Ermine::Object::Counter = 0;
//Ended Static Declarations//

Ermine::Object::Object()
{
	UniqueIdentifier = std::to_string(Counter++);
}
