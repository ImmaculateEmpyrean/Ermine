#include "stdafx.h"
#include "TileMapRendererPrimitive.h"

namespace Ermine
{
	TileMapRendererPrimitive::~TileMapRendererPrimitive()
	{
		//Not Used As Of Now..
	}


	void TileMapRendererPrimitive::SubmitLayerVao(VertexArray Vao)
	{
		LayerVertexArray.emplace_back(Vao);
	}
	void TileMapRendererPrimitive::SubmitLayerVao(int LayerNumber, VertexArray Vao)
	{
		LayerVertexArray.insert(LayerVertexArray.begin() + LayerNumber, Vao);
	}


	void TileMapRendererPrimitive::SubmitLayerTextureToNumberCache(std::unordered_map<std::filesystem::path, float> Cache)
	{
		TextureToNumberCache.emplace_back(Cache);
	}
	void TileMapRendererPrimitive::SubmitLayerTextureToNumberCache(int LayerNumber, std::unordered_map<std::filesystem::path, float> Cache)
	{
		TextureToNumberCache.insert(TextureToNumberCache.begin() + LayerNumber, Cache);
	}


	VertexArray* TileMapRendererPrimitive::GetVao(int LayerNumber)
	{
		if(LayerNumber < LayerVertexArray.size())
			return &LayerVertexArray[LayerNumber];
		else
		{
			STDOUTDefaultLog_Error("Error Layer Number Recieved Exceeds Our Cache.. TileMapRendererPrimitive");
			return nullptr;
		}
	}
	int TileMapRendererPrimitive::GetNumberOfLayersStored()
	{
		return LayerVertexArray.size();
	}

	std::unordered_map<std::filesystem::path, float>TileMapRendererPrimitive::GetTextureToNumberCache(int index)
	{
		return TextureToNumberCache[index];
	}
	

	void TileMapRendererPrimitive::Flush()
	{
		LayerVertexArray.clear();
		TextureToNumberCache.clear();
	}
}
