#include "stdafx.h"
#include "NewTileMap.h"

#include "imgui.h"

#include "EventSystem/Components/RecieverComponent.h"

#include "nlohmann/json.hpp" 
#include "EngineResourceHandlers/EditorDefaultStrings.h"
#include "EngineResourceHandlers/GlobalTextureCache.h"

#ifdef ER_DEBUG_DEVELOP

Ermine::NewTileMap::NewTileMap(std::function<void(std::filesystem::path)> CreateTilesetViewWindowCallable)
	:
	CallableCreateTileSetViewWindow(CreateTilesetViewWindowCallable)
{
	InitializeBuffers();

	auto Lay = Ermine::TileMap::Layer(std::string("Default"));
	Lay.LayerNumber = NewLayerCounter;
	NewLayerCounter++;
	Map.AddLayerToBack(std::move(Lay));

	auto TextureManager = Ermine::GlobalTextureCache::Get();
	TransparentTexture = TextureManager->GetTextureFromFile("Texture/Transparent.png");
	JsonFileIcon = TextureManager->GetTextureFromFile("Texture/JsonFileIcon.png");

	//Start Initializing Default Folder For Searching Tilemaps To Load..
	auto ContextStrings = Ermine::EditorDefaultStrings::Get();
	auto TileMapsPath = ContextStrings->GetValue("TileMapsDefaultPath");
	memcpy(LoadTilesetsPath, TileMapsPath.value_or("Tilemap\\").c_str(), TileMapsPath.value_or("Tilemap\\").length());
	//Ended Initializing Default Folder For Searching Tilemaps To Load..

	/*Ermine::RecieverComponent::Bind(GenCallableFromMethod(&NewTileMap::RecieveTileSelectedEvents), RecieveTileSetSelectedEventsFlag,
		Ermine::EventType::TileSelectedEvent);*/
}

Ermine::NewTileMap::~NewTileMap()
{
	if (NameBuffer != nullptr)
	{
		delete NameBuffer;
		NameBuffer = nullptr;
	}

	if (LayerNameBuffer != nullptr)
	{
		delete LayerNameBuffer;
		LayerNameBuffer = nullptr;
	}

	if (FilePathBuffer != nullptr)
	{
		delete FilePathBuffer;
		FilePathBuffer = nullptr;
	}

	if (LoadTilesetsPath != nullptr)
	{
		delete LoadTilesetsPath;
		LoadTilesetsPath = nullptr;
	}
}

Ermine::NewTileMap::NewTileMap(const NewTileMap& rhs)
{
	HelperCopyTileMapWindow(rhs);
}
Ermine::NewTileMap Ermine::NewTileMap::operator=(const NewTileMap& rhs)
{
	HelperCopyTileMapWindow(rhs);
	return *this;
}

Ermine::NewTileMap::NewTileMap(NewTileMap&& rhs)
{
	HelperMoveTileMapWindow(std::move(rhs));
}
Ermine::NewTileMap Ermine::NewTileMap::operator=(NewTileMap&& rhs)
{
	HelperMoveTileMapWindow(std::move(rhs));
	return *this;
}

void Ermine::NewTileMap::Draw()
{
	std::string WindowName = "Tilemap - \"" + Map.TileMapName + "\"";


	ImGui::Begin(WindowName.c_str());
	
	//Start Set TileMap Name Section
	ImGui::Text("TileMap Name : ");
	ImGui::SameLine();
	ImGui::InputTextWithHint("##GetTileMapNameNewTileMapClass", "Enter The Name of The Tile Map Here", NameBuffer, 100);
	ImGui::SameLine();
	if (ImGui::Button("Set##SetTileMapNameButtonNewTileMapClass"))
	{
		Map.TileMapName = std::string(NameBuffer);
	}
	//Ended Set TileMap Name Section

	ImGui::Separator();

	ImGui::Text("Select Layer");
	ImGui::SameLine();
	static const char* current_item = current_item = Map.Layers[0].Name.c_str();;

	if (ImGui::BeginCombo("##SelectLayerComboBoxNewTileMap", current_item))
	{
		for (int n = 0; n < Map.Layers.size(); n++)
		{
			bool is_selected = (current_item == Map.Layers[n].Name.c_str());
			//bool is_selected;
			if (ImGui::Selectable(Map.Layers[n].Name.c_str(), is_selected))
			{
				current_item = Map.Layers[n].Name.c_str();
				LayerChosen = n;
				memcpy(LayerNameBuffer, Map.Layers[n].Name.c_str(), 100);
			}
			if (is_selected)
			{
				//memset(LayerNameBuffer, 0, 100);
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	ImGui::SameLine();
	if (ImGui::Button("+##NewTileMapAddNewLayerButton"))
	{
		if(!DisplayLayerNameInputWindow)
			OpenLayerNameInputWindow = true;
		
	}
	ImGui::SameLine();
	if (ImGui::Button("-##NewTileMapDeleteLayerButton"))
	{
		if (!DisplayLayerDeletionWindow)
			OpenLayerDeletionWindow = true;

	}

	ImGui::Separator();

	//Start Set TileMap Layer Name Section
	ImGui::Text("Layer Name : ");
	ImGui::SameLine();
	ImGui::InputTextWithHint("##GetTileMapLayerNameNewTileMapClass", "Enter The Name of The Layer Here", LayerNameBuffer, 100);
	ImGui::SameLine();
	if (ImGui::Button("Set##SetTileMapLayerNameButtonNewTileMapClass"))
	{
		Map.Layers[LayerChosen].Name = std::string(LayerNameBuffer);
	}
	//Start Set TileMap Layer Name Section

	ImGui::Text("Number Of Tiles In X : ");
	ImGui::SameLine();

	if (ImGui::InputInt("##NewTileMapGetNumberOfTilesX", &Map.Layers[LayerChosen].NumberOfTilesHorizontal, 1, 10))
	{
		Map.Layers[LayerChosen].LayerData.resize(((Map.Layers[LayerChosen].NumberOfTilesHorizontal) * (Map.Layers[LayerChosen].NumberOfTilesVertical)), 0);
	}

	ImGui::Text("Number Of Tiles In Y : ");
	ImGui::SameLine();
	if (ImGui::InputInt("##NewTileMapGetNumberOfTilesY", &Map.Layers[LayerChosen].NumberOfTilesVertical, 1, 10))
	{
		Map.Layers[LayerChosen].LayerData.resize(((Map.Layers[LayerChosen].NumberOfTilesHorizontal) * (Map.Layers[LayerChosen].NumberOfTilesVertical)), 0);
	}

	ImGui::Text("Tile Width  : ");
	ImGui::SameLine();
	if (ImGui::InputInt("##NewTileMapGetWidth", &Map.Layers[LayerChosen].TileWidth, 1, 10))
	{
		Map.Layers[LayerChosen].LayerData.clear();
		Map.Layers[LayerChosen].LayerData.resize(((Map.Layers[LayerChosen].NumberOfTilesHorizontal) * (Map.Layers[LayerChosen].NumberOfTilesVertical)), 0);
	}

	ImGui::Text("Tile Height : ");
	ImGui::SameLine();
	if (ImGui::InputInt("##NewTileMapGetTileHeight", &Map.Layers[LayerChosen].TileHeight, 1, 10)) 
	{
		Map.Layers[LayerChosen].LayerData.clear();
		Map.Layers[LayerChosen].LayerData.resize(((Map.Layers[LayerChosen].NumberOfTilesHorizontal) * (Map.Layers[LayerChosen].NumberOfTilesVertical)), 0);
	}

	ImGui::Separator();

	if (ImGui::Button("View Tilesets In Use##NewTileMapTilesetsMenu"))
	{
		if (DisplayViewTilesetsInUse == false)
			OpenViewTilesetsInUse = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("View Map Json##NewTileMapTilesetsMenu"))
	{
		if(DisplayViewMapJson == false)
			OpenViewMapJson = true;
	}
	ImGui::SameLine();

	ImGui::Separator();

	if (ImGui::Button("ViewConstructedMap##NewTileMapAddNewLayerButton"))
	{
		if (DisplayConstructedMapWindow == false)
			OpenConstructedMapWindow = true;
	}
	ImGui::Separator();

	if (Map.Layers[LayerChosen].NumberOfTilesHorizontal>0)
	{
		ImGui::SetNextWindowContentSize(ImVec2(Map.Layers[LayerChosen].TileWidth * Map.Layers[LayerChosen].NumberOfTilesHorizontal * 1.3f,0));
		
		//ImVec2 child_size = ImVec2(0, ImGui::GetFontSize() * 20.0f);
		float HeightInY = Map.Layers[LayerChosen].NumberOfTilesVertical * Map.Layers[LayerChosen].TileHeight * 20.0f;

		ImVec2 child_size;

		if(HeightInY < ImGui::GetContentRegionAvail().y -50)
			child_size = ImVec2(0,HeightInY);
		else
			child_size = ImVec2(0, ImGui::GetContentRegionAvail().y - 50);

		ImGui::BeginChild("TileMap##ScrollingRegion", child_size, false, ImGuiWindowFlags_HorizontalScrollbar);

		//Start ToolBar For Convienience Functions
		ImGui::Columns(2);

		SetButtonColorRed();
		if (ImGui::Button("Clear Layer##NewTileMapUIClass"))
		{
			for (auto& i : Map.Layers[LayerChosen].LayerData)
			{
				i = 0;
			}
		}
		ClearButtonColor();

		ImGui::NextColumn();

		//Start Setting Up Eraser Functionality
		static bool InitializeSupressedSprite = true;
		static bool EraserSelected = false;
		static bool EraserUnselected = true;
		static int SupressedSpriteIndex = 0;
		ImGui::Selectable("Tile Eraser##NewTileMapUIClassChild", &EraserSelected);
		if (EraserSelected)
		{
			if (InitializeSupressedSprite)
			{
				SupressedSpriteIndex = SelectedSpriteIndex;
				InitializeSupressedSprite = false;
			}
			EraserUnselected = true;
			SelectedSpriteIndex = 0;
		}
		else
		{
			if (EraserUnselected)
			{
				EraserUnselected = false;
				SelectedSpriteIndex = SupressedSpriteIndex;
				InitializeSupressedSprite = true;
			}
		}
		//Ended Setting Up Eraser Functionality

		ImGui::Separator();
		ImGui::Columns(1);
		//Ended ToolBar For Convienience Functions

		ImGui::Columns(Map.Layers[LayerChosen].NumberOfTilesHorizontal,(const char*)0,false);
		for (int i = 0; i < Map.Layers[LayerChosen].NumberOfTilesHorizontal * Map.Layers[LayerChosen].NumberOfTilesVertical;i++)
		{
			ImGui::PushID(i);

			if (Map.Layers[LayerChosen].LayerData[i] != 0)
			{
				if (ImGui::ImageButton((void*)(intptr_t)Map.GetSprite(Map.Layers[LayerChosen].LayerData[i], LayerChosen)->GetTexture()->GetTextureID(),
					ImVec2(Map.Layers[LayerChosen].TileWidth, Map.Layers[LayerChosen].TileHeight),
					ImVec2(Map.GetSprite(Map.Layers[LayerChosen].LayerData[i], LayerChosen)->GetTopRightUV().x, Map.GetSprite(Map.Layers[LayerChosen].LayerData[i], LayerChosen)->GetTopRightUV().y),
					ImVec2(Map.GetSprite(Map.Layers[LayerChosen].LayerData[i], LayerChosen)->GetBottomLeftUV().x, Map.GetSprite(Map.Layers[LayerChosen].LayerData[i], LayerChosen)->GetBottomLeftUV().y),0))
				{
					Map.Layers[LayerChosen].LayerData[i] = SelectedSpriteIndex;

					if (ImGui::GetIO().MouseClicked[1])
					{
						Map.Layers[LayerChosen].LayerData[i] = 0;
					}
				}
			}
			else
			{
				if (ImGui::Button("##TileButtonNewTileMap", ImVec2(Map.Layers[LayerChosen].TileWidth, Map.Layers[LayerChosen].TileHeight)))
				{
					Map.Layers[LayerChosen].LayerData[i] = SelectedSpriteIndex;

					if (ImGui::GetIO().MouseClicked[1])
					{
						Map.Layers[LayerChosen].LayerData[i] = 0;
					}
				}
			}

			ImGui::PopID();

			ImGui::NextColumn();
		}

		ImGui::EndChild();
		ImGui::Columns(1);
		ImGui::Separator();
	}

	SetButtonColorGreen();
	if (ImGui::Button("Save"))
	{
		if (DisplaySaveMapJsonWindow == false)
			OpenSaveMapJsonWindow = true;
	}
	ClearButtonColor();
	ImGui::SameLine();
	SetButtonColorRed();
	if (ImGui::Button("Cancel"))
		Quit = true;
	ClearButtonColor();
	ImGui::SameLine();

	if (ImGui::Button("Load##NewTileMapLoadTileMap"))
	{
		if (DisplayLoadTileMapWindow == false)
			OpenLoadTileMapWindow = true;
	}

	ImGui::End();

	//Start Child Window Draw Routines..

	if (OpenLayerNameInputWindow)
	{
		DisplayLayerNameInputWindow = true;
		OpenLayerNameInputWindow = false;
	}

	if (DisplayLayerNameInputWindow)
		DrawLayerNameInputWindow();

	if (OpenConstructedMapWindow)
	{
		DisplayConstructedMapWindow = true;
		OpenConstructedMapWindow = false;
	}

	if (DisplayConstructedMapWindow)
		DrawDisplayConstructedMapWindow();

	if (OpenViewMapJson)
	{
		DisplayViewMapJson = true;
		OpenViewMapJson = false;
	}

	if (DisplayViewMapJson)
		DrawViewMapJsonWindow();

	if (OpenSaveMapJsonWindow)
	{
		DisplaySaveMapJsonWindow = true;
		OpenSaveMapJsonWindow = false;
	}

	if (DisplaySaveMapJsonWindow)
		DrawSaveMapJsonWindow();

	if (OpenLoadTileMapWindow)
	{
		DisplayLoadTileMapWindow = true;
		OpenLoadTileMapWindow = false;
	}

	if (DisplayLoadTileMapWindow)
		DrawLoadTileMapWindow();

	if (OpenViewTilesetsInUse)
	{
		DisplayViewTilesetsInUse = true;
		OpenViewTilesetsInUse = false;
	}

	if (DisplayViewTilesetsInUse)
		DrawViewTilesetsInUseWindow();

	if (OpenLayerDeletionWindow)
	{
		DisplayLayerDeletionWindow = true;
		OpenLayerDeletionWindow = false;
	}

	if (DisplayLayerDeletionWindow)
		DrawLayerDeletionWindow();

	//Ended Child Window Draw Routines.. 
}

void Ermine::NewTileMap::DrawLayerNameInputWindow()
{
	ImGui::Begin("Accept Layer Name", (bool*)0, ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::Text("Enter Name Of The New Layer : ");
	ImGui::SameLine();
	ImGui::InputTextWithHint("##NewTilemapInputLayerNameField", "Enter Name For The Layer To Create", LayerNameBuffer, 100);

	SetButtonColorGreen();
	if (ImGui::Button("Confirm"))
	{
		if (LayerNameBuffer[0] != 0)
		{
			auto lay = Ermine::TileMap::Layer(std::string(LayerNameBuffer));
			lay.LayerNumber = NewLayerCounter;
			NewLayerCounter++;
			Map.AddLayerToBack(std::move(lay));
			DisplayLayerNameInputWindow = false;
		}
	}
	ClearButtonColor();
	ImGui::SameLine();
	SetButtonColorRed();
	if (ImGui::Button("Cancel"))
		DisplayLayerNameInputWindow = false;
	ClearButtonColor();

	ImGui::End();
}

void Ermine::NewTileMap::DrawLayerDeletionWindow()
{
	static std::vector<int> BOOlVector(Map.Layers.size(), 0);
	BOOlVector.resize(Map.Layers.size(), 0); //Resize If Not Already That Size.. 

	ImGui::Begin("Layer Deletion Window##NewTilemapWindow");

	ImGui::Text("Layers Contained In Map...");
	
	if (ImGui::Button("Delete Selected"))
	{
		for (int i = 0; i < Map.Layers.size(); i++)
		{
			if (BOOlVector[i]) //it is not zero then delete
			{
				Map.DeleteLayerFromTileMap(i);
				i--;
			}
		}
		DisplayLayerDeletionWindow = false;
		LayerChosen = 0;
	}
	ImGui::SameLine();
	SetButtonColorRed();

	if (ImGui::Button("Close Window"))
		DisplayLayerDeletionWindow = false;

	ClearButtonColor();

	ImGui::Separator();

	ImGui::Columns(2);

	int c = 0;
	for (int i=0;i<Map.Layers.size();i++)
	{
		ImGui::PushID(c++);

		ImGui::Text("%s",Map.Layers[i].Name.c_str());
		
		ImGui::NextColumn();

		ImGui::Checkbox("##NewTileMApDeleteLayercheckbox", (bool*)&(BOOlVector[i]));

		ImGui::NextColumn();

		ImGui::PopID();
	}

	ImGui::End();

	if (DisplayLayerDeletionWindow == false)
	{
		if (Map.Layers.size() == 0)
		{
			Map.AddLayerToBack(Ermine::TileMap::Layer("DefaultGeneratedLayer"));
		}
	}
}

int c = 0;

void Ermine::NewTileMap::DrawDisplayConstructedMapWindow()
{
	ImGui::Begin("OutputWindow");//,(bool*)0,ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
	ImVec2 StartPositionForDrawing = ImGui::GetCursorPos();
	
	/*for (auto i : Map.Layers)
	{
		ImGui::SetCursorPos(Position);
		
		ImGui::SetNextWindowContentSize(ImVec2((i.TileWidth * i.NumberOfTilesHorizontal * 1.3f), 0));
		ImGui::Columns(i.NumberOfTilesHorizontal,(const char*)0,false);
		//ImGui::SetCursorPos(Position);

		for (int j = 0; j < i.LayerData.size(); j++)
		{
			ImGui::PushID(j);
			if (i.LayerData[j] != 0)
			{
				ImGui::Image((void*)(intptr_t)Map.GetSprite(i.LayerData[j])->GetTexture()->GetTextureID(),
					ImVec2(i.TileWidth, i.TileHeight),
					ImVec2(Map.GetSprite(i.LayerData[j])->GetTopRightUV().x, Map.GetSprite(i.LayerData[j])->GetTopRightUV().y),
					ImVec2(Map.GetSprite(i.LayerData[j])->GetBottomLeftUV().x, Map.GetSprite(i.LayerData[j])->GetBottomLeftUV().y));
			}
			else
			{
				ImGui::Image((void*)(intptr_t)TransparentTexture->GetTextureID(),
					ImVec2(i.TileWidth, i.TileHeight),
					ImVec2(1.0f,1.0f),
					ImVec2(0.0f,0.0f));
			}
			SpecializedNextColumn(Position);
			ImGui::PopID();
		}
		ImGui::Separator();
	}*/

	for (auto i : Map.Layers)
	{
		for (int j = 0; j < i.LayerData.size(); j++)
		{
			ImGui::PushID(j);
			FixDrawingPosition(StartPositionForDrawing, j, std::make_pair(i.NumberOfTilesHorizontal, i.NumberOfTilesVertical), std::make_pair(i.TileWidth, i.TileHeight));

			if (i.LayerData[j] != 0)
			{
				ImGui::Image((void*)(intptr_t)Map.GetSprite(i.LayerData[j],i.LayerNumber)->GetTexture()->GetTextureID(),
					ImVec2(i.TileWidth, i.TileHeight),
					ImVec2(Map.GetSprite(i.LayerData[j],i.LayerNumber)->GetTopRightUV().x, Map.GetSprite(i.LayerData[j],i.LayerNumber)->GetTopRightUV().y),
					ImVec2(Map.GetSprite(i.LayerData[j],i.LayerNumber)->GetBottomLeftUV().x, Map.GetSprite(i.LayerData[j],i.LayerNumber)->GetBottomLeftUV().y));
			}
			else
			{
				ImGui::Image((void*)(intptr_t)TransparentTexture->GetTextureID(),
					ImVec2(i.TileWidth, i.TileHeight),
					ImVec2(1.0f,1.0f),
					ImVec2(0.0f,0.0f));
			}
			ImGui::PopID();
		}
	}
	ImGui::PopStyleVar();
	ImGui::End();
}

void Ermine::NewTileMap::FixDrawingPosition(ImVec2 BasePosition, int NumberToDrawAtOneD, std::pair<int, int> NumberOfTilesInXandY, std::pair<int, int> TileDiamensions)
{
	int Copy = NumberToDrawAtOneD;

	int RowCounter = 0;

	while (NumberToDrawAtOneD >= NumberOfTilesInXandY.first)
	{
		NumberToDrawAtOneD = NumberToDrawAtOneD - NumberOfTilesInXandY.first;
		RowCounter = RowCounter + 1;
	}
	RowCounter--; //One Additional is counted for the last time it breaks..

	ImVec2 PreOffset;
	PreOffset.x = 0;//TileDiamensions.first;
	PreOffset.y = TileDiamensions.second;

	ImVec2 Offset;
	Offset.x = BasePosition.x + (TileDiamensions.first * NumberToDrawAtOneD) + PreOffset.x;
	Offset.y = BasePosition.y + (TileDiamensions.second * RowCounter) + PreOffset.y;

	ImVec2 OffsetBox1;
	OffsetBox1.x = Offset.x + ImGui::GetWindowPos().x;
	OffsetBox1.y = Offset.y + ImGui::GetWindowPos().y;

	ImVec2 OffsetBox2;
	OffsetBox2.x = Offset.x + TileDiamensions.first + ImGui::GetWindowPos().x;
	OffsetBox2.y = Offset.y + TileDiamensions.second + ImGui::GetWindowPos().y;

	ImGui::SetCursorPos(OffsetBox1);

	auto globalDrawlist = ImGui::GetWindowDrawList();
	globalDrawlist->AddRect(OffsetBox1, OffsetBox2, ImU32(ImGui::ColorConvertFloat4ToU32(ImVec4(1.0f,1.0f,1.0f,0.3f))));
	ImGui::SetCursorPos(Offset);
}

void Ermine::NewTileMap::DrawViewMapJsonWindow()
{
	ImGui::Begin("JsonFile##NewTileMapUI");

	ImGui::Text("Json Code : ");
	ImGui::Separator();

	ImGui::TextWrapped("%s", Map.GenerateJsonTileMap().c_str());
	

	ImGui::Separator();

	SetButtonColorGreen();
	if (ImGui::Button("Save"))
	{
		if (DisplaySaveMapJsonWindow == false)
			OpenSaveMapJsonWindow = true;
	}
	ClearButtonColor();
	ImGui::SameLine();
	SetButtonColorRed();
	if (ImGui::Button("Cancel"))
		DisplayViewMapJson = false;
	ClearButtonColor();

	ImGui::End();
}

void Ermine::NewTileMap::DrawSaveMapJsonWindow()
{
	ImGui::Begin("Save MapFile",(bool*)0,ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::Text("Save Path : ");
	ImGui::SameLine();
	ImGui::InputTextWithHint("##NewTileMapAcceptSavePathFromUser", "Enter The Path To which The File Must be Saved Along With The Extension .json", FilePathBuffer, 200);

	SetButtonColorGreen();
	if (ImGui::Button("Save##NewTileMapSaveFile"))
	{
		if (FilePathBuffer[0] != 0)
		{
			std::ofstream OutputFile(std::string(FilePathBuffer),std::ios::binary);
			OutputFile << Map.GenerateJsonTileMap();
			OutputFile.clear();

			DisplaySaveMapJsonWindow = false;
		}
	}
	ClearButtonColor();
	ImGui::SameLine();
	SetButtonColorRed();
	if (ImGui::Button("Cancel##NewTileMapCancelSavingFile"))
	{
		DisplaySaveMapJsonWindow = false;
	}
	ClearButtonColor();

	ImGui::End();
}

void Ermine::NewTileMap::DrawLoadTileMapWindow()
{
	std::filesystem::path SP = std::filesystem::path(std::string(LoadTilesetsPath));
	std::error_code Ec;

	std::filesystem::recursive_directory_iterator MapIterator(SP, Ec);

	ImGui::Begin("LoadTileMap");

	ImGui::Columns(3);

	ImGui::NextColumn();
	ImGui::Text("Search Path :");
	ImGui::SameLine();
	ImGui::InputTextWithHint("##NewTileMapLoadTileMapFromFileSearchPath", "Enter The Path To The Directory To Search For The Requested TileMap", LoadTilesetsPath, 200);
	
	ImGui::NextColumn();

	SetButtonColorRed();
	if (ImGui::Button("Quit##NewTileMapLoadButton"))
	{
		DisplayLoadTileMapWindow = false;
	}
	ClearButtonColor();

	ImGui::Columns(1);

	ImGui::Separator();

	ImGui::Columns(2);

	ImGui::Text("Map");
	ImGui::NextColumn();
	ImGui::Text("File Path");
	ImGui::NextColumn();
	ImGui::Separator();

	int c = 0;
	for (auto i : MapIterator)
	{
		if (i.path().u8string().find(".json"))
		{
			ImGui::PushID(c++);
			if (ImGui::ImageButton((void*)(intptr_t)JsonFileIcon->GetTextureID(), ImVec2(ImGui::GetColumnWidth(), JsonFileIcon->GetHeightToMatchAspectRatio(ImGui::GetColumnWidth()))))
			{
				Map = Ermine::TileMap(i.path().u8string().c_str());
				DisplayLoadTileMapWindow = false;
			}
			ImGui::NextColumn();
			ImGui::Text("%s", i.path().u8string().c_str());
			ImGui::NextColumn();
			ImGui::PopID();
		}
	}

	ImGui::End();
}

void Ermine::NewTileMap::DrawViewTilesetsInUseWindow()
{
	ImGui::Begin("Tilesets In Use Window",(bool*)0,ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::Text("Tilesets Used By Current Layer : ");

	ImGui::Separator();

	int c = 0;
	for (auto T : Map.Layers[LayerChosen].TileSetsBuffer)
	{
		ImGui::PushID(c++);

		std::string ButtonName;
		ButtonName = T->GetName() + "##NewTileMapDrawTilesetsInUseWindowTilesetButtonOnlyLayerUsedTilesets";

		if (ImGui::Button(ButtonName.c_str()))
		{
			CallableCreateTileSetViewWindow(T->GetFilePath());
		}

		ImGui::PopID();
	}

	ImGui::Separator();

	ImGui::Text("All Tilesets In Use : ");
	int d = 0;
	for (auto i : Map.Layers)
	{
		ImGui::PushID(c++);
		for (auto T : i.TileSetsBuffer)
		{
			ImGui::PushID(d++);

			std::string ButtonName;
			ButtonName = T->GetName() + "##NewTileMapDrawTilesetsInUseWindowTilesetButton";

			if (ImGui::Button(ButtonName.c_str()))
			{
				CallableCreateTileSetViewWindow(T->GetFilePath());
			}

			ImGui::PopID();
		}
		ImGui::PopID();
	}
	
	ImGui::Separator();

	SetButtonColorRed();
	if (ImGui::Button("Quit"))
	{
		DisplayViewTilesetsInUse = false;
	}
	ClearButtonColor();

	ImGui::End();
}

void Ermine::NewTileMap::Update()
{
	//Empty For Now
}

void Ermine::NewTileMap::RecieveTileSelectedEvents(Ermine::Event* EveObj)
{
	STDOUTDefaultLog_Critical("Haha Recieved TileSelected Events Message Printed From New TileMap");

	TileSelectedEvent* Event = (TileSelectedEvent*)EveObj;

	auto TileDiamensionsFromTileset = Ermine::TileSet::GetTileDiamensionsFromTileset(Event->GetTilesetPath());

	if ((TileDiamensionsFromTileset.first == Map.Layers[LayerChosen].TileWidth) && (TileDiamensionsFromTileset.second == Map.Layers[LayerChosen].TileHeight))
	{
		SelectedSpriteIndex = Event->GetIndex();
		Map.AddTileset(Event->GetTilesetPath(), LayerChosen);
	}
}

void Ermine::NewTileMap::InitializeBuffers()
{
	NameBuffer = new char[100];
	LayerNameBuffer = new char[100];
	FilePathBuffer = new char[200];

	LoadTilesetsPath = new char[200];

	memset(NameBuffer, 0, 100);
	memset(LayerNameBuffer, 0, 100);
	memset(FilePathBuffer, 0, 200);
	memset(LoadTilesetsPath, 0, 200);
}

void Ermine::NewTileMap::HelperCopyTileMapWindow(const NewTileMap& rhs)
{
	CallableCreateTileSetViewWindow = rhs.CallableCreateTileSetViewWindow; //Copy Over The Events To Right..

	Map = rhs.Map;
	SelectedSpriteIndex = rhs.SelectedSpriteIndex;
	LayerChosen = rhs.LayerChosen;

	TransparentTexture = rhs.TransparentTexture;
	JsonFileIcon = rhs.JsonFileIcon;

	OpenTilesetChoosingMenu = rhs.OpenTilesetChoosingMenu;
	DisplayTilesetChoosingMenu = rhs.DisplayTilesetChoosingMenu;

	OpenViewMapJson = rhs.OpenViewMapJson;
	DisplayViewMapJson = rhs.DisplayViewMapJson;

	OpenConstructedMapWindow = rhs.OpenConstructedMapWindow;
	DisplayConstructedMapWindow = rhs.DisplayConstructedMapWindow;

	OpenLayerNameInputWindow = rhs.OpenLayerNameInputWindow;
	DisplayLayerNameInputWindow = rhs.DisplayLayerNameInputWindow;

	OpenLayerDeletionWindow = rhs.OpenLayerDeletionWindow;
	DisplayLayerDeletionWindow = rhs.DisplayLayerDeletionWindow;

	OpenSaveMapJsonWindow = rhs.OpenSaveMapJsonWindow;
	DisplaySaveMapJsonWindow = rhs.DisplaySaveMapJsonWindow;

	OpenLoadTileMapWindow = rhs.OpenLoadTileMapWindow;
	DisplayLoadTileMapWindow = rhs.DisplayLoadTileMapWindow;

	OpenViewTilesetsInUse = rhs.OpenViewTilesetsInUse;
	DisplayViewTilesetsInUse = rhs.DisplayViewTilesetsInUse;

	RecieveTileSetSelectedEventsFlag = rhs.RecieveTileSetSelectedEventsFlag.load();

	NewLayerCounter = rhs.NewLayerCounter;

	InitializeBuffers();
	memcpy(NameBuffer, rhs.NameBuffer, 100);
	memcpy(LayerNameBuffer, rhs.LayerNameBuffer, 100);
	memcpy(FilePathBuffer, rhs.FilePathBuffer, 200);
	memcpy(LoadTilesetsPath, rhs.LoadTilesetsPath, 200);
}

void Ermine::NewTileMap::HelperMoveTileMapWindow(NewTileMap&& rhs)
{
	CallableCreateTileSetViewWindow = rhs.CallableCreateTileSetViewWindow; //Copy Over The Events To Right..

	Map = std::move(rhs.Map);
	SelectedSpriteIndex = rhs.SelectedSpriteIndex;
	LayerChosen = rhs.LayerChosen;

	TransparentTexture = std::move(rhs.TransparentTexture);
	JsonFileIcon = std::move(rhs.JsonFileIcon);

	OpenTilesetChoosingMenu = rhs.OpenTilesetChoosingMenu;
	DisplayTilesetChoosingMenu = rhs.DisplayTilesetChoosingMenu;

	OpenViewMapJson = rhs.OpenViewMapJson;
	DisplayViewMapJson = rhs.DisplayViewMapJson;

	OpenConstructedMapWindow = rhs.OpenConstructedMapWindow;
	DisplayConstructedMapWindow = rhs.DisplayConstructedMapWindow;

	OpenLayerNameInputWindow = rhs.OpenLayerNameInputWindow;
	DisplayLayerNameInputWindow = rhs.DisplayLayerNameInputWindow;

	OpenLayerDeletionWindow = rhs.OpenLayerDeletionWindow;
	DisplayLayerDeletionWindow = rhs.DisplayLayerDeletionWindow;

	OpenSaveMapJsonWindow = rhs.OpenSaveMapJsonWindow;
	DisplaySaveMapJsonWindow = rhs.DisplaySaveMapJsonWindow;

	OpenLoadTileMapWindow = rhs.OpenLoadTileMapWindow;
	DisplayLoadTileMapWindow = rhs.DisplayLoadTileMapWindow;

	OpenViewTilesetsInUse = rhs.OpenViewTilesetsInUse;
	DisplayViewTilesetsInUse = rhs.DisplayViewTilesetsInUse;

	RecieveTileSetSelectedEventsFlag = rhs.RecieveTileSetSelectedEventsFlag.load();

	NewLayerCounter = rhs.NewLayerCounter;

	NameBuffer = rhs.NameBuffer;
	rhs.NameBuffer = nullptr;

	LayerNameBuffer = rhs.LayerNameBuffer;
	rhs.LayerNameBuffer = nullptr;

	FilePathBuffer = rhs.FilePathBuffer;
	rhs.FilePathBuffer = nullptr;

	LoadTilesetsPath = rhs.LoadTilesetsPath;
	rhs.LoadTilesetsPath = nullptr;

	/*Ermine::RecieverComponent::Bind(GenCallableFromMethod(&NewTileMap::RecieveTileSelectedEvents), RecieveTileSetSelectedEventsFlag,
		Ermine::EventType::TileSelectedEvent);*/
}

void Ermine::NewTileMap::SetButtonColor(ImVec4 Color)
{
	ImVec4 ButtonColor = Color;

	Color.x = std::min<float>(255.0f, Color.x * 1.2);
	Color.y = std::min<float>(255.0f, Color.y * 1.2);
	Color.z = std::min<float>(255.0f, Color.z * 1.2);

	ImVec4 ButtonHovered = Color;

	Color.x = std::min<float>(255.0f, Color.x * 1.2);
	Color.y = std::min<float>(255.0f, Color.y * 1.2);
	Color.z = std::min<float>(255.0f, Color.z * 1.2);

	ImVec4 ButtonActive = Color;

	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(255, 255, 255)));
	ImGui::PushStyleColor(ImGuiCol_Button, ButtonColor);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ButtonHovered);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ButtonActive);
}

void Ermine::NewTileMap::SetButtonColorGreen()
{
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(255, 255, 255)));
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 168, 107)));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(ImColor(63, 122, 77)));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(ImColor(41, 171, 135)));
}

void Ermine::NewTileMap::SetButtonColorRed()
{
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(255, 255, 255)));
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(220, 20, 60)));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(ImColor(178, 34, 34)));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(ImColor(139, 0, 0)));
}

void Ermine::NewTileMap::ClearButtonColor()
{
	ImGui::PopStyleColor(4);
}

#endif