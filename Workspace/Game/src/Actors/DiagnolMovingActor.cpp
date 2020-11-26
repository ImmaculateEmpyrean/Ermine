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
	
	//Actor2D::OnUpdateTickEventRecieved();
	
	MovableObject::Translate(1.0f, 1.0f);
	STDOUTDefaultLog_Critical("Hoi");
}

std::shared_ptr<Game::DiagnolActor> Game::DiagnolActor::Generate(std::shared_ptr<Ermine::Actor2DBase> Act)
{
	std::shared_ptr<DiagnolActor> DAct(new DiagnolActor(std::dynamic_pointer_cast<Ermine::Actor2D,Ermine::Actor2DBase>(Act)->GetSprite()));
	return DAct;
}

Game::DiagnolActor::DiagnolActor(DiagnolActor& rhs)
	:
	Actor2D(rhs)
{}

Game::DiagnolActor Game::DiagnolActor::operator=(DiagnolActor& rhs)
{
	Actor2D::operator=(rhs);
	return *this;
}

Game::DiagnolActor::DiagnolActor(DiagnolActor&& rhs)
	:
	Actor2D(std::move(rhs))
{}

Game::DiagnolActor Game::DiagnolActor::operator=(DiagnolActor&& rhs)
{
	Actor2D::operator=(std::move(rhs));
	return *this;
}
