#include "stdafx.h"
#include "TileMapLayerRenderable.h"

Ermine::TileMapLayerRenderable::TileMapLayerRenderable(VertexArray Vao, Material Mat, std::unordered_map<std::filesystem::path, float> TextureAndNumberPaths)
	:
	RenderableTextureModule(std::move(Vao), std::move(Mat)),
	TexturesAndNumbers(std::move(TextureAndNumberPaths))
{

}

Ermine::TileMapLayerRenderable::~TileMapLayerRenderable()
{}

std::vector<int> Ermine::TileMapLayerRenderable::BindTexturesContained()
{
	auto TextureCacheGlobal = Ermine::GlobalTextureCache::Get();

	std::vector<int> TextureMappingUnits;
	TextureMappingUnits.resize(16);

	for (int i = 0; i < 16; i++)
		TextureMappingUnits[i] = 0;

	for (auto Cask = TexturesAndNumbers.begin(); Cask != TexturesAndNumbers.end(); Cask++)
	{
		std::shared_ptr<Texture> Tex = TextureCacheGlobal->GetTextureFromFile(Cask->first);
		int BoundSlot = TextureCacheGlobal->Bind(Tex);

		TextureMappingUnits[(int)Cask->second] = BoundSlot;
	}
	
	return TextureMappingUnits;
}

void Ermine::TileMapLayerRenderable::HelperSetupRenderableTextureModule()
{
	auto TextureCache = Ermine::GlobalTextureCache::Get();
	for (auto i : TexturesAndNumbers)
	{
		std::shared_ptr<Ermine::Texture> Tex = TextureCache->GetTextureFromFile(i.first);
		RenderableTextureModule::SubmitTexture(Tex);
	}
}
