#include "stdafx.h"
#include "Entity.h"

namespace Ermine
{
	Entity::Entity(std::weak_ptr<Ermine::Scene> ScenePointer)
		:
		ScenePointer(ScenePointer)
	{
		if (!ScenePointer.expired())
		{
			std::shared_ptr<Ermine::Scene> StrScenePointer = ScenePointer.lock();
			Id = ((std::uint32_t)StrScenePointer->SceneRegistry.create());
		}
		else STDOUTDefaultLog_Critical("Expired Scene Encountered When Trying To Create Entity.. A Potentially Fatal Error..")
	}
	Entity::Entity(std::uint32_t enttId, std::weak_ptr<Ermine::Scene> ScenePointer)
		:
		Id(enttId),
		ScenePointer(ScenePointer)
	{}
}
