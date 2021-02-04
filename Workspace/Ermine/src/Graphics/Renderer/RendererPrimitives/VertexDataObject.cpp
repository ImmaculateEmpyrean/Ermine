#include "stdafx.h"
#include "VertexDataObject.h"

namespace Ermine
{
	EVertexDataObjectType VertexDataObject::GetDataType()
	{
		if (std::get_if<float>(&Value))
			return EVertexDataObjectType::FLOAT;

		if (std::get_if<unsigned int>(&Value))
			return EVertexDataObjectType::UINT;
		if (std::get_if<int>(&Value))
			return EVertexDataObjectType::INT;

		if (std::get_if<std::uint8_t>(&Value))
			return EVertexDataObjectType::UBYTE;
		if (std::get_if<std::int8_t>(&Value))
			return EVertexDataObjectType::BYTE;
	}

	std::size_t VertexDataObject::GetDataSize()
	{
		if (std::get_if<float>(&Value))
			return sizeof(float);

		if (std::get_if<unsigned int>(&Value))
			return sizeof(unsigned int);
		if (std::get_if<int>(&Value))
			return sizeof(int);

		if (std::get_if<std::uint8_t>(&Value))
			return sizeof(std::uint8_t);
		if (std::get_if<std::int8_t>(&Value))
			return sizeof(std::int8_t);

		else
		{
			STDOUTDefaultLog_Error("Cannot Find The Size Of The VertexData Object.. Returning Zero..File : {1}, Function: {2}, Line : {3}", __func__, __LINE__,__FILE__);
			return 0;
		}
	}


	std::pair<uint8_t*, std::size_t> VertexDataObject::GetVertexDataObject()
	{
		if (auto ptr = std::get_if<float>(&Value))
			return std::make_pair((uint8_t*)ptr, sizeof(float));

		if (auto ptr = std::get_if<unsigned int>(&Value))
			return std::make_pair((uint8_t*)ptr, sizeof(unsigned int));
		if (auto ptr = std::get_if<int>(&Value))
			return std::make_pair((uint8_t*)ptr, sizeof(int));

		if (auto ptr = std::get_if<std::uint8_t>(&Value))
			return std::make_pair((uint8_t*)ptr, sizeof(uint8_t));
		if (auto ptr = std::get_if<std::int8_t>(&Value))
			return std::make_pair((uint8_t*)ptr, sizeof(int8_t));
		else
		{
			STDOUTDefaultLog_Error("Cannot Find The Value Of The VertexData Object.. Returning Zero And NullPointer File : {1}, Function: {2}, Line : {3}", __func__, __LINE__, __FILE__);
			return std::make_pair((uint8_t*)0, 0);
		}
	}
}