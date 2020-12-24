#pragma once
#include<iostream>
#include<vector>
#include<string>

#include "Level/Scene.h"

//Must Include All Components Currently Supported By The Engine..
#include "EntityComponentSystem/Components.h"

namespace Ermine
{
	/*
	* Ermine::Entity Does Not Own entt::entity.. Ermine::Entity Is More Like A Frnt End Wrapper To Entt::Entity Than An Object We Wanna Keep Track Of..
	* Create And Dispose Ermine::Entity At Will On The Stack.. The Actually Entity Component System Is Managed By entt..
	*/

	class Entity
	{
	public:
		//An Empty Entity Cannot Exist As Id Can Also Be Zero.. Basically.. A Supposedly Empty Ermine Entity Would Be Acessing Some entt Entity It Should Not..
		//Validation Flags Maybe Too musc Engineering.. Must See..
		Entity() = delete;
		Entity(std::weak_ptr<Ermine::Scene> ScenePointer);
		Entity(std::uint32_t enttId, std::weak_ptr<Ermine::Scene> ScenePointer);

	public:
		template<typename T, typename ...Params>
		void AddComponent(Params&&... params)
		{
			if (!ScenePointer.expired())
			{
				std::shared_ptr<Ermine::Scene> StrScenePointer = ScenePointer.lock();

				//Get The Registry From The Scene And Call The Emplace Function Which In Turn Calls The Constructor Of Said Component To Construct The Component Inside entt..
				auto& Registry = StrScenePointer->SceneRegistry;
				Registry.emplace<T>(((entt::entity)Id), std::forward<Params>(params)...);
			}
			else STDOUTDefaultLog_Critical("Expired Scene Encountered When Trying To Add Component To Entity.. A Potentially Fatal Error..")
		}

	private:
		std::uint32_t Id;
		std::weak_ptr<Ermine::Scene> ScenePointer;
	};
}