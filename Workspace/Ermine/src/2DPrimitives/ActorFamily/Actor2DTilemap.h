#pragma once
#include<iostream>
#include<vector>
#include<string>

#include "Actor2DShape.h"

#include "Level/TiledLoader/TileMap.h"

namespace Ermine
{
	class Actor2DTilemap : public Actor2DShape
	{
	protected:
		//There Is No Point In Empty Actor2D Tilemap Right..
		Actor2DTilemap() = delete;

		//The Map That Is To Be Drawn Is To Be Given To The Actor..
		Actor2DTilemap(std::shared_ptr<Ermine::TileMap> Map);

	public:
		//The TileMap To Be Loaded Is To Be Given Right..
		static std::shared_ptr<Ermine::Actor2DTilemap> Generate(std::filesystem::path TileMapFile);
		static std::shared_ptr<Ermine::Actor2DTilemap> Generate(std::shared_ptr<Ermine::TileMap> Map);

	public:
		virtual Ermine::ActorFamilyIdentifier GetActorFamilyIdentifier()	  override { return ActorFamilyIdentifier::Actor2DTileMap; }

#pragma region RenderableGenerationImperatives
		//This Function Is Essential For Interaction With Renderable And Its Implemntation Is Necessary For The Concretization Of Any Class..
		virtual int32_t GetRendererDrawMode() override { return GL_TRIANGLES; }
		virtual std::vector<float> GenerateModelSpaceVertexBuffer() override; //This Must Be Implemented Seriously..
		virtual std::vector<uint32_t> GenerateModelSpaceIndices() override; //Thsi Must Be Steup..
#pragma endregion

	private:
		std::shared_ptr<Ermine::TileMap> TileMap;
		int NumberOfVertices = 0;
	};
}