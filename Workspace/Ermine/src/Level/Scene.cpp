#include "stdafx.h"
#include "Scene.h"

namespace Ermine
{
	Scene::Scene()
	{}

	
	Scene::Scene(Scene&& rhs)
	{
		SceneRegistry = std::move(rhs.SceneRegistry);
	}
	Scene& Scene::operator=(Scene&& rhs)
	{
		SceneRegistry = std::move(rhs.SceneRegistry);
		return *this;
	}
}
