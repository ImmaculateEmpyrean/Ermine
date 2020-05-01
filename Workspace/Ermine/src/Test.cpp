#include "stdafx.h"

#include "EventSystem/EventBroadcastStation.h"

#include "TestFolder/RecieverWithBroadcastComponent.h"
#include "TestFolder/SenderWithSendComponent.h"

#include "Log.h"

#include "LayerSystem/LayerStack.h"
#include "LayerSystem/LayerStackLayer.h"

#include "App.h"

void DLL __cdecl TestFunc();

int main(int argc, char* argv[])
{
	auto App = Ermine::App::Get();
	while (true)
	{
		App->NextFrame();
	}
}