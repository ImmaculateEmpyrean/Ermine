#pragma once
#include<iostream>
#include<vector>
#include<unordered_map>
#include<string>
#include<optional>

#include<mutex>

#ifdef ER_DEBUG_DEVELOP 

namespace Ermine
{
	//This Singleton Class Is Only Available In Debug Development mode and is used to get strings which are required by the editor..
	class EditorDefaultStrings
	{
	public:
		EditorDefaultStrings(); //No Need To Pass Parameters But Still Construction Has To Be Done Right..
		~EditorDefaultStrings();

	public:
		static EditorDefaultStrings* Get(); //Use This To Get A Context To Editor Default Strings
		static void DestroyContext();

		void SubmitChanges(std::string Key, std::string Value); //Submit Changes For Ratifaction

		std::optional<std::string> GetValue(std::string Key);

	public:

	protected:

	protected:

	private:
		void ReWriteJsonFileWithLatestValues();

	private:
		std::unordered_map<std::string, std::string> StringCache; //This Holds All The Strings Required to be used by the editors

		//Start Static Things
		static EditorDefaultStrings* SingleTonPointer;
		static std::once_flag InitializationFlag;
		//Ended Static Things
	};
}

#endif