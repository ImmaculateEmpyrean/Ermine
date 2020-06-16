#include "stdafx.h"
#include "TileMap.h"

#include "ErmineJsonFunctions.h"

namespace Ermine
{
	TileMap::TileMap(std::filesystem::path TileMapFilePath)
		:
		TileMapPath(TileMapFilePath)
	{
		LoadTileMapFromPath();
	}

	TileMap::~TileMap()
	{
		for (int i = 0; i < TileSetsBuffer.size(); i++)
		{
			if(TileSetsBuffer[i] != nullptr) //No Point In Deleting a Nullptr Right..
				delete TileSetsBuffer[i];
		}

	}


	TileMap::TileMap(const TileMap& rhs)
	{
		TileMapPath = rhs.TileMapPath;
		LoadTileMapFromPath();
	}
	TileMap TileMap::operator=(const TileMap& rhs)
	{
		TileMapPath = rhs.TileMapPath;
		LoadTileMapFromPath();

		return *this;
	}

	TileMap::TileMap(TileMap&& rhs)
	{
		TileMapPath = rhs.TileMapPath;
		TileMapName = rhs.TileMapName;

		Layers = std::move(rhs.Layers);

		TileSetsBuffer = std::move(rhs.TileSetsBuffer);

		TileSetStartIndexTracker = rhs.TileSetStartIndexTracker;
	}
	TileMap TileMap::operator=(TileMap&& rhs)
	{
		TileMapPath = rhs.TileMapPath;
		TileMapName = rhs.TileMapName;

		Layers = std::move(rhs.Layers);

		TileSetsBuffer = std::move(rhs.TileSetsBuffer);

		TileSetStartIndexTracker = rhs.TileSetStartIndexTracker;

		return *this;
	}


	bool TileMap::operator==(TileMap&& rhs)
	{
		return TileMapPath == rhs.TileMapPath;
	}


	int TileMap::GetStartIndex()
	{
		return 1;
	}
	int TileMap::GetEndIndex()
	{
		return TileSetStartIndexTracker[TileSetStartIndexTracker.size() - 1] + TileSetsBuffer[TileSetsBuffer.size() - 1]->GetNumberOfSpritesInTileSet();
	}

	Ermine::Sprite* TileMap::GetSprite(int Index)
	{
		//This Still Does Not Work..
		//We WIll Simply Check RAnge

		int c = 0;

		while (true)
		{
			if (Index < TileSetEndIndexTracker[c] && Index >= TileSetStartIndexTracker[c])
				break;
			c++;
		}
		
		return TileSetsBuffer[c]->GetTile(Index);
	}


	void TileMap::LoadTileMapFromPath()
	{
		//See That The TileMap FilePath Is Not Empty At ANy cost...
		assert(!TileMapPath.empty());

		std::ifstream FileRaw(TileMapPath);

		nlohmann::json TileSetFile;
		TileSetFile << FileRaw;

		TileMapName = TileSetFile["TileMapName"].dump();

		//Start Extracting Layers//
		for (auto i = TileSetFile["Layers"].begin(); i != TileSetFile["Layers"].end(); i++)
		{
			Layer Container;

			Container.Name = i.key();

			Container.TileWidth = std::stoi((*i)["TileWidthPixels"].dump());
			Container.TileHeight = std::stoi((*i)["TileHeightPixels"].dump());

			Container.NumberOfTilesHorizontal = std::stoi((*i)["NumberOfTilesHorizontal"].dump());
			Container.NumberOfTilesVertical = std::stoi((*i)["NumberOfTilesVertical"].dump());

			Container.LayerNumber = std::stoi((*i)["LayerNumber"].dump());

			Container.LayerData = ExtractIntDataFromJsonArray((*i)["TileData"].dump());

			Layers.emplace_back(Container);
		}
		//Ended Extracting Layers//

		//Start Extracting TileSets//
		for (auto i = TileSetFile["TileSet"].begin(); i != TileSetFile["TileSet"].end(); i++)
		{
			std::string ExtractedPath = i.key();

			std::ifstream TileSetFileInputRaw(ExtractedPath);

			nlohmann::json TileSetJsonFile;
			TileSetJsonFile << TileSetFileInputRaw;

			int NumberOfTiles = std::stoi(TileSetJsonFile["NumberOfTiles"].dump());

			TileSetsBuffer.emplace_back(new TileSet(std::filesystem::path(ExtractedPath)));

			TileSetStartIndexTracker.emplace_back(std::stoi(TileSetFile["TileSet"][ExtractedPath.c_str()]["StartIndex"].dump()));
			TileSetEndIndexTracker.emplace_back(NumberOfTiles + TileSetStartIndexTracker[TileSetStartIndexTracker.size()-1]);
		}
		//Ended Extracting TileSets//

		CreateRendererFriendlyDrawable();
	}

	void TileMap::CreateRendererFriendlyDrawable()
	{
		RendererFriendlyDrawable.Flush(); //Clear This If Something Does Exist..

		std::vector<Ermine::VertexArray> VaoArray;
		std::vector<std::unordered_map<std::filesystem::path, float>> TextureMappingCache;

		for (auto Layer : this->GetAllLayers())
		{
			auto casket = CreateVertexArrayForLayer(Layer);
			VaoArray.emplace_back(casket.first);
			TextureMappingCache.emplace_back(casket.second);
		}
		
		for (auto i : VaoArray)
			RendererFriendlyDrawable.SubmitLayerVao(i);

		for (auto i : TextureMappingCache)
			RendererFriendlyDrawable.SubmitLayerTextureToNumberCache(i);
	}

	std::pair<VertexArray, std::unordered_map<std::filesystem::path, float>> Ermine::TileMap::CreateVertexArrayForLayer(Ermine::TileMap::Layer& layer)
	{
		int GridXToBeGenerated = layer.NumberOfTilesHorizontal;
		int GridYToBeGenerated = layer.NumberOfTilesVertical;

		float StepInX = Ermine::GetScreenHeight() / NumberOfGridsInXOnScreen;
		float StepInY = Ermine::GetScreenWidth() / NumberOfGridsInYOnScreen;

		//These Variables Are USed To Generate The VErtex Buffer..
		float CurrentPositionX = 0.0f;
		float CurrentPositionY = 0.0f;

		//This VAriable is used to generate the index buffer...
		int IndexCounter = 0;

		//This Number Is Used To Count The Texture Number...
		int TextureNumber = 0;

		std::vector<float> VertexBuffer;
		std::vector<uint32_t> IndexBuffer;

		std::unordered_map<std::filesystem::path, float> TextureToNumberMapper; //We Must Return This Too I Guess...

		int Count = 0;
		for (auto i : layer.LayerData)
		{
			if (i == 0)
			{
				//Are You Stupid All You Are Gonna GEt Is Diagnol...
				CurrentPositionX = CurrentPositionX + StepInX;
				Count++;
				if (Count >= layer.NumberOfTilesHorizontal)
				{
					Count = 0;
					CurrentPositionX = 0.0f;
					CurrentPositionY = CurrentPositionY + StepInY;
				}
				//CurrentPositionY = CurrentPositionY + StepInY;

				continue;
			}

			//Start GEtting The Index Buffer Ready..
			IndexBuffer.emplace_back(IndexCounter);
			IndexBuffer.emplace_back(IndexCounter + 1);
			IndexBuffer.emplace_back(IndexCounter + 3);

			IndexBuffer.emplace_back(IndexCounter + 1);
			IndexBuffer.emplace_back(IndexCounter + 2);
			IndexBuffer.emplace_back(IndexCounter + 3);

			IndexCounter = IndexCounter + 4;
			//Ended Getting The Index Buffer Ready..

			auto TextureNumberMapperIterator = TextureToNumberMapper.find(this->GetSprite(i)->GetTexture()->GetFilePath());
			if (TextureNumberMapperIterator == TextureToNumberMapper.end())
			{
				//Not Found
				TextureToNumberMapper[this->GetSprite(i)->GetTexture()->GetFilePath()] = TextureNumber++;
			}

			//Start Setting Up Top Right Vertex..
			VertexBuffer.emplace_back(CurrentPositionX + StepInX); //x
			VertexBuffer.emplace_back(CurrentPositionY + StepInY); //y
			VertexBuffer.emplace_back(0.0f); //z

			VertexBuffer.emplace_back(this->GetSprite(i)->GetTopRightUV().x); //u
			VertexBuffer.emplace_back(this->GetSprite(i)->GetTopRightUV().y); //v

			VertexBuffer.emplace_back(TextureToNumberMapper.find(this->GetSprite(i)->GetTexture()->GetFilePath()).operator*().second); //Texture Number
			//Ended Setting Up Top Right Vertex..

			//Start Setting Up Bottom Right Vertex..
			VertexBuffer.emplace_back(CurrentPositionX + StepInX);
			VertexBuffer.emplace_back(CurrentPositionY);
			VertexBuffer.emplace_back(0.0f);

			VertexBuffer.emplace_back(this->GetSprite(i)->GetTopRightUV().x);
			VertexBuffer.emplace_back(this->GetSprite(i)->GetBottomLeftUV().y);

			VertexBuffer.emplace_back(TextureToNumberMapper.find(this->GetSprite(i)->GetTexture()->GetFilePath()).operator*().second); //Texture Number
			//Ended Setting Up Bottom Right Vertex..

			//Start Setting Up Bottom Left Vertex..
			VertexBuffer.emplace_back(CurrentPositionX);
			VertexBuffer.emplace_back(CurrentPositionY);
			VertexBuffer.emplace_back(0.0f);

			VertexBuffer.emplace_back(this->GetSprite(i)->GetBottomLeftUV().x);
			VertexBuffer.emplace_back(this->GetSprite(i)->GetBottomLeftUV().y);

			VertexBuffer.emplace_back(TextureToNumberMapper.find(this->GetSprite(i)->GetTexture()->GetFilePath()).operator*().second); //Texture Number
			//Ended Setting Up Bottom Left Vertex..

			//Start Setting Up Top Left Vertex..
			VertexBuffer.emplace_back(CurrentPositionX);
			VertexBuffer.emplace_back(CurrentPositionY + StepInY);
			VertexBuffer.emplace_back(0.0f);

			VertexBuffer.emplace_back(this->GetSprite(i)->GetBottomLeftUV().x);
			VertexBuffer.emplace_back(this->GetSprite(i)->GetTopRightUV().y);

			VertexBuffer.emplace_back(TextureToNumberMapper.find(this->GetSprite(i)->GetTexture()->GetFilePath()).operator*().second); //Texture Number
			//Ended Setting Up Top Left Vertex..

			CurrentPositionX = CurrentPositionX + StepInX;

			Count++;
			if (Count >= layer.NumberOfTilesHorizontal)
			{
				Count = 0;
				CurrentPositionX = 0.0f;
				CurrentPositionY = CurrentPositionY + StepInY;
			}

		}
		return std::make_pair(Ermine::VertexArray(VertexBuffer, IndexBuffer), TextureToNumberMapper);
	}
}
