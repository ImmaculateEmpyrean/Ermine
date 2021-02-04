#pragma once
#include<iostream>
#include<vector>
#include<string>

#include<optional>
#include<variant>

#include<type_traits>

#include "Log/Log.h"

#include "EVertexDataObjectType.h"

namespace Ermine
{
	

	class VertexDataObject 
	{
		typedef std::variant<float, std::uint32_t, std::int32_t, std::uint8_t, std::int8_t> VERTEX_BUFFER_DATATYPE;
	public:
		typedef std::int8_t ERMINE_BYTE;
		typedef std::uint8_t UNSIGNED_ERMINE_BYTE;

	public:
		VertexDataObject() = default;
		template<typename T> VertexDataObject(T Value)
		{
			//Assign Said Value To Variant.. Throw A Compile Time Error If Variant Cannot Accept Said Type..
			this->Value = Value;
		}

	private:
		VERTEX_BUFFER_DATATYPE Value = 0.0f; //Dunno Default variant Behaviour As Of This Writing..

	public:
		//get the type of data this object holds..
		EVertexDataObjectType GetDataType();
		
		//get the size of the data said object holds.. (bytes as sizeof is being used under hood)
		std::size_t GetDataSize();

		template<typename T>
		std::optional<T> GetValue()
		{
			if (std::is_same<T, float>::value)
			{
				if (auto pval = std::get_if<float>(&Value))
					return *pval;
				else return {};
			}

			if (std::is_same<T, unsigned int>::value)
			{
				if (auto pval = std::get_if<std::uint32_t>(&Value))
					return *pval;
				else return {};
			}
			if (std::is_same<T, int>::value)
			{
				if (auto pval = std::get_if<std::int32_t>(&Value))
					return *pval;
				else return {};
			}

			if (std::is_same<T, ERMINE_BYTE>::value)
			{
				if (auto pval = std::get_if<ERMINE_BYTE>(&Value))
					return *pval;
				else return {};
			}
			if (std::is_same<T, UNSIGNED_ERMINE_BYTE>::value)
			{
				if (auto pval = std::get_if<UNSIGNED_ERMINE_BYTE>(&Value))
					return *pval;
				else return {};
			}

			STDOUTDefaultLog_Error("The Template Argument Type Is Not Supported By The Vertex Buffer");
			return {}
		}

		//This Function Returns A Pointer To The Start Of The Object And The Size Of The Object.. Better When You Donot Know The Type And Are Simply Trying To Pass Along To Somewhere..
		std::pair<uint8_t*, std::size_t> GetVertexDataObject();

		//Extremely Type Safe As The Compiler Throws An Error If I Try To Assign Random Types..
		template<typename T>
		void SetValue(T Value) { this->Value = Value; }
	};
}