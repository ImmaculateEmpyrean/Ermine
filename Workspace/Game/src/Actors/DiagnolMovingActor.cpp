#include "stdafx.h"
#include "DiagnolMovingActor.h"

Game::DiagnolActor::DiagnolActor(std::shared_ptr<Ermine::Sprite> Sprite)
	:
	Actor2D(Sprite)
{}

Game::DiagnolActor::DiagnolActor(std::shared_ptr<Ermine::Actor2D> Act)
	:
	Actor2D(*Act)
{}

void Game::DiagnolActor::OnUpdateTickEventRecieved()
{
	auto Lock = GetObjectMutex();
	MovableObject::Translate(1.0f, 1.0f);
}

std::shared_ptr<Ermine::Actor2DBase> Game::DiagnolActor::Generate(std::shared_ptr<Ermine::Actor2DBase> Act)
{
	std::shared_ptr<DiagnolActor> DAct(new DiagnolActor(std::dynamic_pointer_cast<Ermine::Actor2D,Ermine::Actor2DBase>(Act)));

	return DAct;
}
