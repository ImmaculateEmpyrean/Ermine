#include "Gstdafx.h"

#include "App.h"
#include "EntryPoint.h"

//#include ""

/*#include <iostream>
#include <vector>
#include <string>

#include "App.h"

#include "EntryPoint.h"

#include "EventSystem/EventBroadcastStation.h"

#include "Actors/DiagnolMovingActor.h"

//#include "2DPrimitives/ActorFamily/TiledActor2D.h"

class GameApp : public Ermine::App
{
public:
	GameApp()
		:
		Ermine::App("Game",{1920,1080},std::filesystem::path("Levels/TestLevel.json"))
	{}
};

Ermine::App* Ermine::ExecuteApp()
{
	return new GameApp();
}*/

std::shared_ptr<Ermine::Actor2DBase> Ermine::Actor2DConstructionCallback(std::shared_ptr<Ermine::Actor2DBase> Act)
{
	//std::shared_ptr<Game::DiagnolActor> Act2 = Game::DiagnolActor::Generate(Act);
	return Act;
}

class GameApp : public Ermine::App
{
public:
	GameApp()
		:
		Ermine::App("Game", { 1920,1080 })
	{}
};

Ermine::App* Ermine::ExecuteApp()
{
	return new GameApp();
}