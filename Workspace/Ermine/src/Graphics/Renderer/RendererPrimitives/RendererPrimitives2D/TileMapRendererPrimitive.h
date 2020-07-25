#pragma once
#include<iostream>
#include<vector>
#include<string>

#include "EngineResourceHandlers/GlobalTextureCache.h"

#include "../VertexArray.h"
#include "../VertexBuffer.h"
#include "../IndexBuffer.h"

//#include "LayerSystem/LayerStack.h"
//#include "LayerSystem/LayerStackLayer.h"

namespace Ermine
{
	class TileMapRendererPrimitive
	{
	public:
		//This Wont Ever Be Used Only For The Renderer To Store Data..
		TileMapRendererPrimitive() = default; 

		//Might Need a Custom Destructor Dunno Yet..
		~TileMapRendererPrimitive();

	public:
		//This Places The Vao At The Back Of The Queue Layer Number Is Automagically Assigned..
		void SubmitLayerVao(VertexArray Vao);
		
		//This Allows One To Specify The Layer Explicitly..
		void SubmitLayerVao(int LayerNumber,VertexArray Vao);

		//This Places The Cache At The Back Of The Queue Layer Number Is Automagically Assigned..
		void SubmitLayerTextureToNumberCache(std::unordered_map<std::filesystem::path, float> Cache);

		//This Allows One To Specify The Layer Explicitly..
		void SubmitLayerTextureToNumberCache(int LayerNumber, std::unordered_map<std::filesystem::path, float> Cache);

		//Allows One To Access Whichever Vao Was Needed...
		VertexArray* GetVao(int LayerNumber);
		int GetNumberOfLayersStored();

		std::unordered_map<std::filesystem::path, float> GetTextureToNumberCache(int index);

		void Flush(); //This Clears The Contents of This Object..

		//LayerStackLayer GetLayerAsLayerStackLayer(int LayerNumber);
		//LayerStack GetAllLayersAsLayerStack();

	protected:

	protected:

	private:

	private:
		//Here Index Is Mapped To The Layer Number..
		std::vector<VertexArray> LayerVertexArray;

		std::vector<std::unordered_map<std::filesystem::path, float>> TextureToNumberCache;
	};
}