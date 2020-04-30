#include "stdafx.h"
#include "App.h"

#include "ExchangeStructs/ExchangeAppEvents.h"

Ermine::App::App(std::string AppTitle, std::pair<int, int> Diamensions)
	:
	AppTitle(AppTitle),
	Diamensions(Diamensions)
{
	Obj = GetAppEventsStruct();
	OnAttach(); //This Event Is Called Signifying That The App Is Now Attached...
}

Ermine::App::~App()
{
	OnDetach(); //The Event Is Called...
}

void Ermine::App::NextFrame()
{
	//TODO Carefully Implement This Function.....
}

void Ermine::App::OnAttach()
{
	Obj.OnAttach();
}

void Ermine::App::OnTick()
{
	Obj.OnTick();
}

void Ermine::App::OnDetach()
{
	Obj.OnDetach();
}
