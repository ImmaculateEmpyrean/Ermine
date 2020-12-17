#include "stdafx.h"
#include "TileSet.h"

#include<cmath>

#include "EngineResourceHandlers/GlobalTextureCache.h"

static float normalize(float val,float min,float max) 
{
	// Shift to positive to avoid issues when crossing the 0 line
	if (min < 0) {
		max += 0 - min;
		val += 0 - min;
		min = 0;
	}
	// Shift values from 0 - max
	val = val - min;
	max = max - min;
	return std::max(0.0f, std::min(1.0f, val / max));
}

namespace Ermine
{
	//The Problem Is Somewhere In This Function Probably..
	TileSet::TileSet(std::string TileSetName,std::shared_ptr<Ermine::Sprite> TileSet, std::pair<int, int> TileDiamensions, int StartIndex)
	{
		//The Indexes Start From 1..
		SpritesInTheTileset.emplace_back(Ermine::Sprite::GetNullSprite());

		std::shared_ptr<Ermine::Texture> TileSetTexture = TileSet->GetTexture();
		
		int Counter = 0;

		int RightExtreme = TileSet->GetTopRightUV().x * TileSetTexture->GetWidth();
		int LeftExtreme = TileSet->GetBottomLeftUV().x * TileSetTexture->GetWidth();

		int TopExtreme = TileSet->GetBottomLeftUV().y * TileSetTexture->GetHeight();
		int BottomExtreme = TileSet->GetTopRightUV().y * TileSetTexture->GetHeight();



		for (int j = TopExtreme; j < BottomExtreme; j = j + TileDiamensions.second)
		{
			for (int i = LeftExtreme; i < RightExtreme; i = i + TileDiamensions.first)
			{
				std::shared_ptr<Ermine::Sprite> Spr = Ermine::Sprite::GenerateSprite(TileSetTexture,
					{ normalize(i , 0, TileSet->GetTopRightUV().x * TileSetTexture->GetWidth()), 1.0f - normalize(j + TileDiamensions.second , 0, TileSet->GetTopRightUV().y * TileSetTexture->GetHeight()) },
					{ normalize(i + TileDiamensions.first, 0, TileSet->GetTopRightUV().x * TileSetTexture->GetWidth()), 1.0f - normalize(j, 0, TileSet->GetTopRightUV().y * TileSetTexture->GetHeight()) }); 

				SpritesInTheTileset.emplace_back(Spr);

				Counter++;
			}
		}

		this->TileSetName = TileSetName;

		TileWidth = TileDiamensions.first;
		TileHeight = TileDiamensions.second;

		this->StartIndex = StartIndex;

		TileSetSprite = TileSet;

		EndIndex = StartIndex + Counter;
	}
	TileSet::~TileSet()
	{}


	std::shared_ptr<Ermine::TileSet> TileSet::Generate(std::string TileSetName,std::shared_ptr<Ermine::Sprite> TileSet, std::pair<int, int> TileDiamensions, int StartIndex)
	{
		std::shared_ptr<Ermine::TileSet> Set(new Ermine::TileSet(TileSetName,TileSet,TileDiamensions,StartIndex));
		return Set;
	}


	std::string TileSet::GetName()
	{
		return TileSetName;
	}
	void TileSet::SetName(std::string TileSetName)
	{
		this->TileSetName = TileSetName;
	}


	std::shared_ptr<Sprite> TileSet::GetTile(int index)
	{
		index = index - StartIndex + 1; //Well Since The Start Index Is One So The Index Must Start From One Right..
		if (index < 0)
			STDOUTDefaultLog_Critical("Error Sprite Access Index Less Than 0.. Will Certainly Crash The Program..")
		
		return SpritesInTheTileset[index];
	}


	std::shared_ptr<Sprite> TileSet::GetTileSetImage()
	{
		return TileSetSprite;
	}

	std::vector<std::shared_ptr<Sprite>> TileSet::GetSpriteBuffer()
	{
		return SpritesInTheTileset;
	}
	int TileSet::GetNumberOfSpritesInTileSet()
	{
		return SpritesInTheTileset.size();
	}


	std::pair<int, int> TileSet::GetTileDiamensions()
	{
		return std::make_pair(GetTileWidth(),GetTileHeight());
	}
	int TileSet::GetTileWidth()
	{
		return TileWidth;
	}
	int TileSet::GetTileHeight()
	{
		return TileHeight;
	}


	int Ermine::TileSet::GetStartIndex()
	{
		return StartIndex;
	}
	int TileSet::GetEndIndex()
	{
		return EndIndex;
	}
}
