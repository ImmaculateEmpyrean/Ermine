#include "stdafx.h"

#include <iostream>
#include <vector>
#include <string>

#include "App.h"

#include "EntryPoint.h"

#include "EventSystem/EventBroadcastStation.h"

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
	auto Station = Ermine::EventBroadcastStation::GetStation();

	std::shared_ptr<Ermine::Actor2D> Act2 = Ermine::Actor2D::GenerateActor2D("Texture/ErmineNullTexture.png", { 500.0f,500.0f });
	//std::shared_ptr<Game::DiagnolActor> Dact = Game::DiagnolActor::Generate(Act);
	return Act2;//Dact;
}