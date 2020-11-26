#pragma once
#include<iostream>
#include<vector>
#include<string>

#include "2DPrimitives/Sprite.h"
#include "2DPrimitives/ActorFamily/Actor2D.h"

#include "glm.hpp"

namespace Game
{
	class DiagnolActor:public Ermine::Actor2D
	{
	public:
		DiagnolActor(std::shared_ptr<Ermine::Sprite> Sprite);

		DiagnolActor(std::shared_ptr<Ermine::Actor2D> Act);

		virtual ~DiagnolActor()
		{

		}

		virtual void OnUpdateTickEventRecieved() override;

		static std::shared_ptr<Game::DiagnolActor> Generate(std::shared_ptr<Ermine::Actor2DBase> Act);

		DiagnolActor(DiagnolActor& rhs);
		DiagnolActor operator=(DiagnolActor& rhs);

		DiagnolActor(DiagnolActor&& rhs);
		DiagnolActor operator=(DiagnolActor&& rhs);
	};
}