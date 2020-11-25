#include "stdafx.h"

#include <iostream>
#include <vector>
#include <string>

#include "App.h"

#include "EntryPoint.h"

#include "Actors/DiagnolMovingActor.h"

class GameApp : public Ermine::App
{
public:
	GameApp()
		:
		Ermine::App("Game",{1000,1000},std::filesystem::path("Levels/TestLevel.json"))
	{}
};

Ermine::App* Ermine::ExecuteApp()
{
	return new GameApp();
}

std::shared_ptr<Ermine::Actor2DBase> Ermine::Actor2DConstructionCallback(std::shared_ptr<Ermine::Actor2DBase> Act)
{
	auto Dact = Game::DiagnolActor::Generate(Act);
	return Dact;
}