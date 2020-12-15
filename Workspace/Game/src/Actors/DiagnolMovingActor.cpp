#include "stdafx.h"
#include "DiagnolMovingActor.h"

Game::DiagnolActor::DiagnolActor(std::shared_ptr<Ermine::Sprite> Sprite)
	:
	Actor2D(std::vector<std::shared_ptr<Ermine::Sprite>>(1,Sprite),glm::mat4(1.0f))
{}

Game::DiagnolActor::DiagnolActor(std::shared_ptr<Ermine::Actor2D> Act)
	:
	Actor2D(std::move(*Act))
{}

void Game::DiagnolActor::OnUpdateTickEventRecieved()
{
	auto Lock = GetObjectMutex();
	MovableObject::Translate(1.0f, 1.0f);
}

std::shared_ptr<Game::DiagnolActor> Game::DiagnolActor::Generate(std::shared_ptr<Ermine::Actor2DBase> Act)
{
	std::shared_ptr<DiagnolActor> DAct(new DiagnolActor(std::dynamic_pointer_cast<Ermine::Actor2D,Ermine::Actor2DBase>(Act)));
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
