#include "stdafx.h"
#include "TileMap.h"

#include "ErmineJsonFunctions.h"

namespace Ermine
{
	TileMap::TileMap()
	{
		//Finish This Next Session
	}
	TileMap::TileMap(std::filesystem::path TileMapFilePath)
		:
		TileMapPath(TileMapFilePath)
	{
		LoadTileMapFromPath();
	}

	TileMap::~TileMap()
	{}


	TileMap::TileMap(const TileMap& rhs)
	{
		TileMapPath = rhs.TileMapPath;
		
		if(!(TileMapPath.empty()))
			LoadTileMapFromPath();
	}
	TileMap TileMap::operator=(const TileMap& rhs)
	{
		TileMapPath = rhs.TileMapPath;

		if (!(TileMapPath.empty()))
			LoadTileMapFromPath();

		return *this;
	}

	TileMap::TileMap(TileMap&& rhs)
	{
		TileMapPath = rhs.TileMapPath;
		TileMapName = rhs.TileMapName;

		Layers = std::move(rhs.Layers);

		//TileSetsBuffer = std::move(rhs.TileSetsBuffer);

		//TileSetStartIndexTracker = rhs.TileSetStartIndexTracker;

		TilemapFullyFunctional = rhs.TilemapFullyFunctional;
	}
	TileMap TileMap::operator=(TileMap&& rhs)
	{
		TileMapPath = rhs.TileMapPath;
		TileMapName = rhs.TileMapName;

		Layers = std::move(rhs.Layers);

		//TileSetsBuffer = std::move(rhs.TileSetsBuffer);

		//TileSetStartIndexTracker = rhs.TileSetStartIndexTracker;

		TilemapFullyFunctional = rhs.TilemapFullyFunctional;

		return *this;
	}


	bool TileMap::operator==(TileMap&& rhs)
	{
		return TileMapPath == rhs.TileMapPath;
	}


	int TileMap::GetStartIndex(int LayerNumber)
	{
		return 1;
	}
	int TileMap::GetEndIndex(int LayerNumber)
	{
		return Layers[LayerNumber].GetEndIndex();//TileSetStartIndexTracker[Layers[LayerNumber].TileSetStartIndexTracker.size() - 1] + Layers[LayerNumber].TileSetsBuffer[Layers[LayerNumber].TileSetsBuffer.size() - 1]->GetNumberOfSpritesInTileSet();
	}

	std::shared_ptr<Sprite> TileMap::GetSprite(int Index,int LayerNumber)
	{
		//This Still Does Not Work..
		//We WIll Simply Check RAnge

		int c = 0;

		while (true)
		{
			if (Index <= Layers[LayerNumber].TileSetEndIndexTracker[c] && Index >= Layers[LayerNumber].TileSetStartIndexTracker[c]) //Maybe make <= to < if it does not work..
				break;
			c++;
		}
		
		return Layers[LayerNumber].TileSetsBuffer[c]->GetTile(Index - Layers[LayerNumber].TileSetStartIndexTracker[c]); //Maybe Remove - TilesetIndexTracker[c] if it does not work..
	}

	void TileMap::SetTileValue(int LayerNumber, int TileIndex,int TileValue)
	{
		Layers[LayerNumber].LayerData[TileIndex] = TileValue;
	}
	int TileMap::GetTileValue(int LayerNumber, int TileIndex)
	{
		return Layers[LayerNumber].LayerData[TileIndex];
	}


	void TileMap::AddLayerToBack(Ermine::TileMap::Layer LayerToAdd)
	{
		Layers.emplace_back(LayerToAdd);
	}

	void TileMap::AddLayerToFront(Ermine::TileMap::Layer LayerToAdd)
	{
		Layers.insert(Layers.begin(), LayerToAdd);
	}

	void TileMap::AddTileset(std::filesystem::path TilesetPath,int LayerNumber)
	{
		HelperAddTileset(std::make_unique<Ermine::TileSet>(TilesetPath),LayerNumber);
	}
	void TileMap::AddTileset(std::unique_ptr<Ermine::TileSet> TilesetToAdd,int LayerNumber)
	{
		HelperAddTileset(std::move(TilesetToAdd),LayerNumber);
	}

	void TileMap::HelperAddTileset(std::unique_ptr<Ermine::TileSet> TilesetPtr,int LayerNumber)
	{
		if (!HelperCheckIfTilesetExists(TilesetPtr->GetFilePath(),LayerNumber))
		{
			if (Layers[LayerNumber].TileSetEndIndexTracker.size() == 0)
			{
				Layers[LayerNumber].TileSetStartIndexTracker.emplace_back(1);
				Layers[LayerNumber].TileSetEndIndexTracker.emplace_back(TilesetPtr->GetSpriteBuffer().size());
			}
			else
			{
				Layers[LayerNumber].TileSetStartIndexTracker.emplace_back(Layers[LayerNumber].TileSetEndIndexTracker[Layers[LayerNumber].TileSetEndIndexTracker.size() - 1]);
				Layers[LayerNumber].TileSetEndIndexTracker.emplace_back(((Layers[LayerNumber].TileSetEndIndexTracker[Layers[LayerNumber].TileSetEndIndexTracker.size() - 1]) + TilesetPtr->GetNumberOfSpritesInTileSet()));
				//Layers[LayerNumber].TileSetStartIndexTracker.emplace_back(Layers[LayerNumber].TileSetEndIndexTracker[(Layers[LayerNumber].TileSetEndIndexTracker.size() - 2)] + 1);
				//Layers[LayerNumber].TileSetEndIndexTracker.emplace_back(TilesetPtr->GetSpriteBuffer().size() + Layers[LayerNumber].TileSetStartIndexTracker[Layers[LayerNumber].TileSetStartIndexTracker.size() - 1]);
			}

			Layers[LayerNumber].TileSetsBuffer.emplace_back(TilesetPtr.release());
		}
	}


	std::string TileMap::GenerateJsonTileMap()
	{
		
			nlohmann::json JsonFile;
			std::stringstream JsonHolder;
		try
		{
			JsonFile["TileMapName"] = TileMapName;

			for (auto i : Layers)
			{
				nlohmann::json Layer;
				nlohmann::json LayerProperties;

				LayerProperties["TileWidthPixels"] = i.TileWidth;
				LayerProperties["TileHeightPixels"] = i.TileHeight;

				LayerProperties["NumberOfTilesHorizontal"] = i.NumberOfTilesHorizontal;
				LayerProperties["NumberOfTilesVertical"] = i.NumberOfTilesHorizontal;
				LayerProperties["TileData"] = i.LayerData; //Check This I Have Got My Doubts.. [Already Checked This Seems To Work]
				LayerProperties["LayerNumber"] = i.LayerNumber;

				Layer[i.Name] = LayerProperties;
				JsonFile["Layers"].push_back(Layer);	
			}
			
			std::stringstream JsonTextureHolder;

			
			for (auto l : Layers)
			{
				for (auto i : l.TileSetsBuffer)
				{
					int c = 0;

					nlohmann::json TileSet;
					nlohmann::json TileSetProperties;

					TileSetProperties["StartIndex"] = l.TileSetStartIndexTracker[c++];

					TileSet[i->GetFilePath().u8string()] = TileSetProperties;

					JsonFile["TileSet"].push_back(TileSet);
				}

			}
		}
		catch (...)
		{

		}
			return JsonFile.dump();	
	}
	

	void TileMap::LoadTileMapFromPath()
	{
		//See That The TileMap FilePath Is Not Empty At ANy cost...
		assert(!TileMapPath.empty());

		std::ifstream FileRaw(TileMapPath);

		nlohmann::json TileSetFile;
		TileSetFile << FileRaw;

		TileMapName = TileSetFile["TileMapName"].dump();

		/*//Start Extracting Layers//
		for (auto i : TileSetFile["Layers"].items())  //for (auto i = TileSetFile["Layers"].begin(); i != TileSetFile["Layers"].end(); i++)
		{
			Layer Container = Layer("Def");

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
		for(auto i : TileSetFile["TileSet"].items())//for (auto i = TileSetFile["TileSet"].begin(); i != TileSetFile["TileSet"].end(); i++)
		{
			std::string ExtractedPath = i.key();

			std::ifstream TileSetFileInputRaw(ExtractedPath);

			nlohmann::json TileSetJsonFile;
			TileSetJsonFile << TileSetFileInputRaw;

			int NumberOfTiles = std::stoi(TileSetJsonFile["NumberOfTiles"].dump());

			for (auto& l : Layers)
			{
				if ((l.TileWidth == Ermine::TileSet::GetTileDiamensionsFromTileset(std::filesystem::path(ExtractedPath)).first) && (l.TileHeight == Ermine::TileSet::GetTileDiamensionsFromTileset(std::filesystem::path(ExtractedPath)).second))
				{

					l.TileSetsBuffer.emplace_back(new TileSet(std::filesystem::path(ExtractedPath)));

					l.TileSetStartIndexTracker.emplace_back(std::stoi(TileSetFile["TileSet"][ExtractedPath.c_str()]["StartIndex"].dump()));
					l.TileSetEndIndexTracker.emplace_back(NumberOfTiles + l.TileSetStartIndexTracker[l.TileSetStartIndexTracker.size() - 1]);
				}
			}
		}
		//Ended Extracting TileSets//*/


		//Start Extracting Layers//
		int LayerNumber = 0;
		for (auto& it : TileSetFile["Layers"].items())  //for (auto i = TileSetFile["Layers"].begin(); i != TileSetFile["Layers"].end(); i++)
		{
			auto object = it.value();
			
			Layer Container = Layer("Def");

			
			Container.Name = it.value().items().begin().key();

			//std::cout << it.value().find("TileWidthPixels").value().dump();
			std::cout << it.value();

			Container.TileWidth = std::stoi(it.value().begin().value().find("TileWidthPixels").value().dump());
			Container.TileHeight = std::stoi(it.value().begin().value().find("TileHeightPixels").value().dump());

			Container.NumberOfTilesHorizontal = std::stoi(it.value().begin().value().find("NumberOfTilesHorizontal").value().dump());
			Container.NumberOfTilesVertical = std::stoi(it.value().begin().value().find("NumberOfTilesVertical").value().dump());

			Container.LayerNumber = std::stoi(it.value().begin().value().find("LayerNumber").value().dump());

			Container.LayerData = ExtractIntDataFromJsonArray(it.value().begin().value().find("TileData").value().dump());

			Layers.emplace_back(Container);

		}
		//Ended Extracting Layers//

		//Start Extracting TileSets//

		/*for(auto i : TileSetFile["TileSet"].items())//for (auto i = TileSetFile["TileSet"].begin(); i != TileSetFile["TileSet"].end(); i++)
		{
			std::string ExtractedPath = i.key();

			std::ifstream TileSetFileInputRaw(ExtractedPath);

			nlohmann::json TileSetJsonFile;
			TileSetJsonFile << TileSetFileInputRaw;

			int NumberOfTiles = std::stoi(TileSetJsonFile["NumberOfTiles"].dump());

			for (auto& l : Layers)
			{
				if ((l.TileWidth == Ermine::TileSet::GetTileDiamensionsFromTileset(std::filesystem::path(ExtractedPath)).first) && (l.TileHeight == Ermine::TileSet::GetTileDiamensionsFromTileset(std::filesystem::path(ExtractedPath)).second))
				{
					l.TileSetsBuffer.emplace_back(new TileSet(std::filesystem::path(ExtractedPath)));

					l.TileSetStartIndexTracker.emplace_back(std::stoi(TileSetFile["TileSet"][ExtractedPath.c_str()]["StartIndex"].dump()));
					l.TileSetEndIndexTracker.emplace_back(NumberOfTiles + l.TileSetStartIndexTracker[l.TileSetStartIndexTracker.size() - 1]);
				}
			}
		}*/

		for(auto it : TileSetFile["TileSet"].items())//for (auto i = TileSetFile["TileSet"].begin(); i != TileSetFile["TileSet"].end(); i++)
		{
			std::string ExtractedPath = it.value().items().begin().key();

			std::ifstream TileSetFileInputRaw(ExtractedPath);

			nlohmann::json TileSetJsonFile;
			TileSetJsonFile << TileSetFileInputRaw;

			int NumberOfTiles = std::stoi(TileSetJsonFile["NumberOfTiles"].dump());

			for (auto& l : Layers)
			{
				if ((l.TileWidth == Ermine::TileSet::GetTileDiamensionsFromTileset(std::filesystem::path(ExtractedPath)).first) && (l.TileHeight == Ermine::TileSet::GetTileDiamensionsFromTileset(std::filesystem::path(ExtractedPath)).second))
				{
					l.TileSetsBuffer.emplace_back(new TileSet(std::filesystem::path(ExtractedPath)));

					std::cout << it.value().dump()<< std::endl;//.find("StartIndex").value().dump();
					l.TileSetStartIndexTracker.emplace_back(std::stoi(it.value().begin().value().find("StartIndex").value().dump()));
					l.TileSetEndIndexTracker.emplace_back(NumberOfTiles + l.TileSetStartIndexTracker[l.TileSetStartIndexTracker.size() - 1]);
				}
			}
		}

		//Ended Extracting TileSets//

		CreateRendererFriendlyDrawable();

		TilemapFullyFunctional = true; //This Flag is Used To Set That Tilemap Can Be Used As Intended..
	}

	void TileMap::WriteTileMapToDisk()
	{
		std::stringstream Stream;
		auto SourceCode = GenerateJsonTileMap();
		Stream << SourceCode;

		std::string FilePath = "TileMap/";
		FilePath = FilePath + TileMapName;
		FilePath = FilePath + ".json";

		std::ofstream OutputFile(FilePath);

		nlohmann::json OutputJsonFile;
		OutputJsonFile << Stream;

		OutputFile << OutputJsonFile;

		OutputFile.close();
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

			auto TextureNumberMapperIterator = TextureToNumberMapper.find(this->GetSprite(i,layer.LayerNumber)->GetTexture()->GetFilePath());
			if (TextureNumberMapperIterator == TextureToNumberMapper.end())
			{
				//Not Found
				TextureToNumberMapper[this->GetSprite(i, layer.LayerNumber)->GetTexture()->GetFilePath()] = TextureNumber++;
			}

			//Start Setting Up Top Right Vertex..
			VertexBuffer.emplace_back(CurrentPositionX + StepInX); //x
			VertexBuffer.emplace_back(CurrentPositionY + StepInY); //y
			VertexBuffer.emplace_back(0.0f); //z

			VertexBuffer.emplace_back(this->GetSprite(i,layer.LayerNumber)->GetTopRightUV().x); //u
			VertexBuffer.emplace_back(this->GetSprite(i, layer.LayerNumber)->GetTopRightUV().y); //v

			VertexBuffer.emplace_back(TextureToNumberMapper.find(this->GetSprite(i, layer.LayerNumber)->GetTexture()->GetFilePath()).operator*().second); //Texture Number
			//Ended Setting Up Top Right Vertex..

			//Start Setting Up Bottom Right Vertex..
			VertexBuffer.emplace_back(CurrentPositionX + StepInX);
			VertexBuffer.emplace_back(CurrentPositionY);
			VertexBuffer.emplace_back(0.0f);

			VertexBuffer.emplace_back(this->GetSprite(i, layer.LayerNumber)->GetTopRightUV().x);
			VertexBuffer.emplace_back(this->GetSprite(i, layer.LayerNumber)->GetBottomLeftUV().y);

			VertexBuffer.emplace_back(TextureToNumberMapper.find(this->GetSprite(i,layer.LayerNumber)->GetTexture()->GetFilePath()).operator*().second); //Texture Number
			//Ended Setting Up Bottom Right Vertex..

			//Start Setting Up Bottom Left Vertex..
			VertexBuffer.emplace_back(CurrentPositionX);
			VertexBuffer.emplace_back(CurrentPositionY);
			VertexBuffer.emplace_back(0.0f);

			VertexBuffer.emplace_back(this->GetSprite(i, layer.LayerNumber)->GetBottomLeftUV().x);
			VertexBuffer.emplace_back(this->GetSprite(i, layer.LayerNumber)->GetBottomLeftUV().y);

			VertexBuffer.emplace_back(TextureToNumberMapper.find(this->GetSprite(i,layer.LayerNumber)->GetTexture()->GetFilePath()).operator*().second); //Texture Number
			//Ended Setting Up Bottom Left Vertex..

			//Start Setting Up Top Left Vertex..
			VertexBuffer.emplace_back(CurrentPositionX);
			VertexBuffer.emplace_back(CurrentPositionY + StepInY);
			VertexBuffer.emplace_back(0.0f);

			VertexBuffer.emplace_back(this->GetSprite(i, layer.LayerNumber)->GetBottomLeftUV().x);
			VertexBuffer.emplace_back(this->GetSprite(i, layer.LayerNumber)->GetTopRightUV().y);

			VertexBuffer.emplace_back(TextureToNumberMapper.find(this->GetSprite(i, layer.LayerNumber)->GetTexture()->GetFilePath()).operator*().second); //Texture Number
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


	bool TileMap::HelperCheckIfTilesetExists(std::filesystem::path TileSetPath,int LayerNumber)
	{
		for (auto i : Layers[LayerNumber].TileSetsBuffer)
		{
			if (i->GetFilePath() == TileSetPath)
				return true;
		}
		return false;
	}


	TileMap::Layer::Layer(const Layer& rhs)
	{
		HelperCopyFunction(rhs);
	}
	TileMap::Layer TileMap::Layer::operator=(const Layer& rhs)
	{
		HelperCopyFunction(rhs);
		return *this;
	}

	TileMap::Layer::Layer(Layer&& rhs)
	{
		HelperMoveFunction(std::move(rhs));
	}
	TileMap::Layer TileMap::Layer::operator=(Layer&& rhs)
	{
		HelperMoveFunction(std::move(rhs));
		return *this;
	}


	int TileMap::Layer::GetStartIndex()
	{
		return 1;
	}
	int TileMap::Layer::GetEndIndex()
	{
		return TileSetStartIndexTracker[TileSetStartIndexTracker.size() - 1] + TileSetsBuffer[TileSetsBuffer.size() - 1]->GetNumberOfSpritesInTileSet();
	}


	void TileMap::Layer::HelperCopyFunction(const Layer& rhs)
	{
		Name = rhs.Name;

		TileWidth = rhs.TileWidth;
		TileHeight = rhs.TileHeight;

		NumberOfTilesHorizontal = rhs.NumberOfTilesHorizontal;
		NumberOfTilesVertical = rhs.NumberOfTilesVertical;

		LayerNumber = rhs.LayerNumber;

		LayerData = rhs.LayerData;

		for (auto i : rhs.TileSetsBuffer)
			TileSetsBuffer.emplace_back(new Ermine::TileSet(i->GetFilePath()));

		TileSetStartIndexTracker = rhs.TileSetStartIndexTracker;
		TileSetEndIndexTracker = rhs.TileSetEndIndexTracker;
	}

	void TileMap::Layer::HelperMoveFunction(Layer&& rhs)
	{
		Name = rhs.Name;

		TileWidth = rhs.TileWidth;
		TileHeight = rhs.TileHeight;

		NumberOfTilesHorizontal = rhs.NumberOfTilesHorizontal;
		NumberOfTilesVertical = rhs.NumberOfTilesVertical;

		LayerNumber = rhs.LayerNumber;

		LayerData = rhs.LayerData;

		TileSetsBuffer.resize(rhs.TileSetsBuffer.size());
		for (auto& i : rhs.TileSetsBuffer) //Check This Function In The Future rhs.TilesetsBuffer Must Show nullotr otherwise the destructor shall behave dangerously by deleting everything and making the handle copied redundant
		{
			TileSetsBuffer.emplace_back(i);
			i = nullptr;
		}

		TileSetStartIndexTracker = rhs.TileSetStartIndexTracker;
		TileSetEndIndexTracker = rhs.TileSetEndIndexTracker;
	}
}
