#pragma once
#include<iostream>
#include<vector>
#include<string>

#include "EngineResourceHandlers/GlobalTextureCache.h"

#include "2DPrimitives/PrimitiveType2D.h"
#include "Graphics/Renderer/RenderableComponents/RenderableTextureModule.h"

namespace Ermine
{
	class Renderer2D;

	class TileMapLayerRenderable : public Ermine::RenderableTextureModule//Ermine::Renderable2D
	{
	public:
		//There is No Reason For This Constructor Exist And Yet it Does..
		TileMapLayerRenderable() = default; 

		//This Is The MAin Constructor Try Using This More Often
		TileMapLayerRenderable(VertexArray Vao, Material Mat, std::unordered_map<std::filesystem::path, float> TextureAndNumberPaths);

		virtual ~TileMapLayerRenderable();

	public:
		virtual Ermine::PrimitiveType2D GetType() { return PrimitiveType2D::TileMap; }

	public:

	protected:

	protected:

	private:
		virtual std::vector<float> BindTexturesContained() override;

		void HelperSetupRenderableTextureModule();

	private:
		std::unordered_map<std::filesystem::path, float> TexturesAndNumbers;

		friend class Ermine::Renderer2D;
	};
}