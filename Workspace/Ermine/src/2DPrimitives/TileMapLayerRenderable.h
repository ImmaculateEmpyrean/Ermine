#pragma once
#include<iostream>
#include<vector>
#include<string>

#include "Renderable2D.h"

namespace Ermine
{
	class Renderer2D;

	class TileMapLayerRenderable : public Ermine::Renderable2D
	{
	public:
		//There is No Reason For This Constructor Exist And Yet it Does..
		TileMapLayerRenderable() = default; 

		//This Is The MAin Constructor Try Using This More Often
		TileMapLayerRenderable(VertexArray Vao, Material Mat, std::unordered_map<std::filesystem::path, float> TextureAndNumberPaths);

		virtual ~TileMapLayerRenderable();

	public:
		virtual Renderable2DType GetType() { return Renderable2DType::TileMap; }

	public:

	protected:

	protected:

	private:

	private:
		std::unordered_map<std::filesystem::path, float> TexturesAndNumbers;

		friend class Ermine::Renderer2D;
	};
}