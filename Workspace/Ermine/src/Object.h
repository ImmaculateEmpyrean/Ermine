#pragma once
#include<iostream>
#include<vector>
#include<string>

namespace Ermine
{
	class Object
	{
	public:
		//Object Can Be Constructed Without Much Prior Construction For Now.. It Just Cant Be Constructed Defaultly
		Object();

	public:
		std::string GetUniqueIdentifier() { return UniqueIdentifier; }
		
	public:

	protected:

	protected:

	private:
		virtual void Dummy() {}

	private:
		static int Counter;
		std::string UniqueIdentifier;
	};
}