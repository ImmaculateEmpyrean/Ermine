#pragma once
#include<iostream>
#include<vector>
#include<unordered_map>
#include<string>
#include<optional>

#include<mutex>

#include "nlohmann/json.hpp"

namespace Ermine
{
	class Manifest
	{
	private:
		//This Is A Singleton Maybe Perform Some Custom Construction.. Dunno..
		Manifest();
		virtual ~Manifest() = default;

		//Start - There Is No Reason To Either Delete Or Copy A Manifest//
		Manifest(const Manifest& rhs) = delete;
		Manifest& operator=(const Manifest& rhs) = delete;

		Manifest(Manifest&& rhs) = delete;
		Manifest& operator=(Manifest&& rhs) = delete;
		//Ended - There Is No Reason To Either Delete Or Copy A Manifest//

	public:
		//Access The Singleton Using This Context..
		static Manifest* Get(); 

		//Destroy The Singleton.. Possibly before The Engine Shuts Down For Good.. Be Careful Though As Get Will Be Returning A Null Pointer..
		static void DestroyContext();

		//Start Get Or Write A String From The Manifest//
		static std::optional<std::string> GetString(std::string Key);
		static void WriteString(std::string Key, std::string Value);
		//Ended Get Or Write A String From The Manifest//

		static nlohmann::json& GetUnderlyingJson();
	public:

	protected:

	protected:

	private:
		

	private:
		static Manifest* ManifestPointer;
		static std::once_flag InitializationFlag;

		nlohmann::json JsonFile;
	};
}