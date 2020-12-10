#include "stdafx.h"
#include "Manifest.h"

#include "nlohmann/json.hpp"

//start Static Declarations
Ermine::Manifest* Ermine::Manifest::ManifestPointer= nullptr;
std::once_flag Ermine::Manifest::InitializationFlag;
//Ended Static Declarations

namespace Ermine
{
	Manifest::Manifest()
	{
		//Start Initialize The Json File..//
		std::ifstream File("Manifest.json");
		File >> JsonFile;
		//Ended Initialize The Json File..//
	}


	Manifest* Manifest::Get()
	{
		std::call_once(InitializationFlag, []() {
			ManifestPointer = new Ermine::Manifest();
		});

		return ManifestPointer;
	}

	void Manifest::DestroyContext()
	{
		if (ManifestPointer != nullptr)
			delete ManifestPointer;
	}


	std::optional<std::string> Manifest::GetString(std::string Key)
	{
		auto man = Ermine::Manifest::Get();
		for (auto i : man->JsonFile.items())
		{
			if (i.key() == Key)
			{
				std::string Value = i.value().dump();
				Value.erase(std::remove(Value.begin(), Value.end(), '\"'), Value.end());
				return Value;
			}
		}
		
		//Cannot Find The Key..
		STDOUTDefaultLog_Error("Cannot Find Requested Key In Manifest %s", Key.c_str());
		return {};
	}
	void Manifest::WriteString(std::string Key, std::string Value)
	{
		auto man = Ermine::Manifest::Get();
		man->JsonFile[Key] = Value;

		std::fstream OutputFile("Manifest.json");
		OutputFile << man->JsonFile;
		OutputFile.close();
	}


	nlohmann::json& Manifest::GetUnderlyingJson()
	{
		auto man = Ermine::Manifest::Get();
		return man->JsonFile;
	}
}
