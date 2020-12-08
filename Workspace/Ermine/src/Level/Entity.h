#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<filesystem>

#include "Object.h"

#include "sol/assert.hpp"
#include "sol/sol.hpp"

namespace Ermine {

	//An Entity Is Able To Load And Process Lua Scripts..

	class Entity : public Ermine::Object
	{
	public:
		//Well Entity Cannot Be Constructed Default Right Since Atleast The State Is Tobe Initialized..
		Entity();

		//The Virtual Destructor Is Needed For Proper Destructor Dispatch
		virtual ~Entity() override;

		Entity(Entity& rhs);
		Entity& operator=(Entity& rhs);

		Entity(Entity&& rhs);
		Entity& operator=(Entity&& rhs);

	public:
		void SetScript(std::string Action, std::filesystem::path ScriptPath);
		void RemoveScript(std::string Action);

		std::filesystem::path GetScriptPath(std::string Action);
		std::string			  GetScript	   (std::string Action);

		

	public:

	protected:

	protected:

	private:

	private:
		//The Lua State Which You Know Initializes Functions And The This Reference..
		sol::state* LuaState = nullptr;

		//The Buffer Stores Scripts So That They May Be Loaded And Used When Needed
		std::unordered_map<std::string, std::filesystem::path> ScriptsForActions;
	};

}