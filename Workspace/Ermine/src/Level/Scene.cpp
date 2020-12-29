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


	Ermine::Entity* Scene::GetPrimaryCamera()
	{
#if defined(ER_DEBUG_DEVELOP) || defined(ER_DEBUG_SHIP)
		if (PrimaryCamEntity == nullptr)
			STDOUTDefaultLog_Error("Requested Primary Camera Entity When It Was Not Set Prior");
#endif
		return PrimaryCamEntity;
	}
}
