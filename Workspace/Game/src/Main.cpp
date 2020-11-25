#include "stdafx.h"

#include "ExchangeStructs/ExchangeAppEvents.h"
#include "ExchangeStructs/LevelInitialization.h"
#include "ExchangeStructs/ExePointerDeclarations.h"

#include "App.h"
#include "Actors/DiagnolMovingActor.h"
#include "Graphics/Renderer/Renderer2D.h"

static Ermine::App* HApp = nullptr;

BOOL WINAPI DllMain(_In_ HINSTANCE hinstDLL,_In_ DWORD fdwReason,_In_ LPVOID lpvReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		//The DLL Is Clearly Attaching We May Use This Chance Initialize Stuff..
		std::vector<std::pair<std::string, CreateLogFile>> Configuration;
		Configuration.emplace_back(std::make_pair<std::string, CreateLogFile>("Game", CreateLogFile::CreateLogFile));
		Log::Init(Configuration);

		Log_Trace("Game", "Default Level Loaded  Simply Tracing It.. :>");
	}
	
	return TRUE;
}

void DLL _cdecl InitializeDLL(InitializaDLLStruct Struct)
{
	HApp = Struct.App;
	HApp->Message = "Set in DLL Duh";
}

extern void DLL _cdecl Checker()
{
	STDOUTDefaultLog_Info("Checker Has Run In DLL");
	STDOUTDefaultLog_Info(HApp->Message);
	HApp->CallFromDll();
}

extern void DLL _cdecl InitializeScene()
{
	/*auto Spr = Ermine::Sprite::GenerateSprite(std::filesystem::path("AnoHiMitaHana.png"));
	
	std::shared_ptr<Game::DiagnolActor> Act = std::make_shared<Game::DiagnolActor>(Spr);

	Ermine::LayerStackLayer Layer("Hannna");
	Layer.SubmitActor(Act);

	auto Renderer = Ermine::Renderer2D::Get();
	Renderer->SubmitLayer(std::move(Layer));*/
}
