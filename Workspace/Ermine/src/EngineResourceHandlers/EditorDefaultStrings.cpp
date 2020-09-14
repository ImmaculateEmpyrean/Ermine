#include "stdafx.h"
#include "EditorDefaultStrings.h"

#include "nlohmann/json.hpp"

#ifdef ER_DEBUG_DEVELOP

//start Static Declarations
Ermine::EditorDefaultStrings* Ermine::EditorDefaultStrings::SingleTonPointer = nullptr;
std::once_flag Ermine::EditorDefaultStrings::InitializationFlag;
//Ended Static Declarations

Ermine::EditorDefaultStrings::EditorDefaultStrings()
{
	std::ifstream InputFile("EditorDefaultStrings.json");
	nlohmann::json JsonFile;
	JsonFile << InputFile;

	for (auto i : JsonFile.items())
	{
		StringCache[i.key()] = i.value();
	}
}

Ermine::EditorDefaultStrings::~EditorDefaultStrings()
{}

Ermine::EditorDefaultStrings* Ermine::EditorDefaultStrings::Get()
{
	std::call_once(InitializationFlag, []() {
		SingleTonPointer = new EditorDefaultStrings();
	});
	return SingleTonPointer;
}

void Ermine::EditorDefaultStrings::DestroyContext()
{
	if(SingleTonPointer != nullptr)
		delete SingleTonPointer;
	SingleTonPointer = nullptr;
}

void Ermine::EditorDefaultStrings::SubmitChanges(std::string Key, std::string Value)
{
	StringCache[Key] = Value;
	ReWriteJsonFileWithLatestValues();
}

std::optional<std::string> Ermine::EditorDefaultStrings::GetValue(std::string Key)
{
	auto casket = StringCache.find(Key);
	if (casket == StringCache.end())
	{
		return {};
	}
	else
		return casket->second;
}

void Ermine::EditorDefaultStrings::ReWriteJsonFileWithLatestValues()
{
	std::ofstream OutFile("EditorDefaultStrings.json");
	OutFile.clear();

	OutFile << "{\n";

	for (auto i = StringCache.begin();i!=StringCache.end();)
	{
		OutFile << "\"" << i->first << "\"" << ":" << "\"" << i->second << "\"";
		i++;
		if (i == StringCache.end())
			break;
		else
			OutFile << ",\n";
		
	}
	OutFile << "\n}";

	OutFile.close();
}

#endif
