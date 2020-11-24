#pragma once
#include<iostream>
#include<vector>
#include<string>

#include "2DPrimitives/Sprite.h"
#include "2DPrimitives/ActorFamily/Actor2D.h"

namespace Game
{
	class DiagnolActor:public Ermine::Actor2D
	{
	public:
		DiagnolActor(std::shared_ptr<Ermine::Sprite> Sprite);

		virtual void OnUpdateTickEventRecieved() override;
	};
}