#include "stdafx.h"
#include "DiagnolMovingActor.h"

Game::DiagnolActor::DiagnolActor(std::shared_ptr<Ermine::Sprite> Sprite)
	:
	Actor2D(Sprite)
{}

void Game::DiagnolActor::OnUpdateTickEventRecieved()
{
	auto Lock = GetObjectMutex();
	MovableObject::Translate(1.0f, 1.0f);
}
