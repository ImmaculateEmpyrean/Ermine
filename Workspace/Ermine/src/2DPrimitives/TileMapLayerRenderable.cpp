#include "stdafx.h"
#include "TileMapLayerRenderable.h"

Ermine::TileMapLayerRenderable::TileMapLayerRenderable(VertexArray Vao, Material Mat, std::unordered_map<std::filesystem::path, float> TextureAndNumberPaths)
	:
	Renderable2D(std::move(Vao),std::move(Mat)),
	TexturesAndNumbers(std::move(TextureAndNumberPaths))
{}

Ermine::TileMapLayerRenderable::~TileMapLayerRenderable()
{}
