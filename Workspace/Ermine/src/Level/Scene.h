#pragma once
#include<iostream>
#include<vector>
#include<string>

#include "entt.hpp"

/*
* The Scene Class Contains All The Entities That Are Relevant At Any Given Moment.. 
* A Scene With Possibly A Script Component Can Possibly Become A Level.. Something To Think About In The Future..
*/

namespace Ermine
{
	//Forward Declared So That We Can Make Entity A Friend Of Scene Allowing The Entity To Access The Registry At Will..
	class Entity;

	class Scene
	{
	public:
		//An Empty Scene Will Be Constructed For Now.. Just Not A Default One..
		Scene();

		//We Donot Know Yet What It Is To Copy A Scene.. Its Better We Visualize That First Before Allowing Myself To Copy A Scene..
		Scene(const Scene& rhs) = delete;
		Scene& operator=(const Scene& rhs) = delete;

		//The Scene Can Be Moved.. I Dont Think this IS That Difficult..
		Scene(Scene&& rhs);
		Scene& operator=(Scene&& rhs);

	private:
		//The Scene Uses entt in the backend to manage all entities..
		entt::registry SceneRegistry; //TODO- See That Access To Entity Is Thread Safe.. If Not Already Refer To entt..
		
		//Entity Is To Access Scene Registry At Will..
		friend class Ermine::Entity;
	};
}