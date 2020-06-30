#include "stdafx.h"
#include "NewTileMap.h"

#include "imgui.h"

#include "EventSystem/Components/RecieverComponent.h"

#include "nlohmann/json.hpp" 

Ermine::NewTileMap::NewTileMap()
{
	InitializeBuffers();

	auto Lay = Ermine::TileMap::Layer(std::string("Default"));
	Lay.LayerNumber = NewLayerCounter;
	NewLayerCounter++;
	Map.AddLayerToBack(std::move(Lay));

	auto TextureManager = Ermine::GlobalTextureCache::Get();
	TransparentTexture = TextureManager->GetTextureFromFile("Texture/Transparent.png");
	
	Ermine::RecieverComponent::Bind(GenCallableFromMethod(&NewTileMap::RecieveTileSelectedEvents), RecieveTileSetSelectedEventsFlag, 
								    Ermine::EventType::TileSelectedEvent);
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
		OpenTilesetChoosingMenu = true;
	ImGui::SameLine();
	if (ImGui::Button("View Map XML##NewTileMapTilesetsMenu"))
		ViewMapJson = true;
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
		
		ImVec2 child_size = ImVec2(0, ImGui::GetFontSize() * 20.0f);
		ImGui::BeginChild("TileMap##ScrollingRegion", child_size, false, ImGuiWindowFlags_HorizontalScrollbar);
		ImGui::Columns(Map.Layers[LayerChosen].NumberOfTilesHorizontal);
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
				}
			}
			else
			{
				if (ImGui::Button("##TileButtonNewTileMap", ImVec2(Map.Layers[LayerChosen].TileWidth, Map.Layers[LayerChosen].TileHeight)))
				{
					Map.Layers[LayerChosen].LayerData[i] = SelectedSpriteIndex;
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
	ImGui::Button("Save");
	ClearButtonColor();
	ImGui::SameLine();
	SetButtonColorRed();
	if (ImGui::Button("Cancel"))
		Quit = true;
	ClearButtonColor();

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

	memset(NameBuffer, 0, 100);
	memset(LayerNameBuffer, 0, 100);
}

void Ermine::NewTileMap::HelperCopyTileMapWindow(const NewTileMap& rhs)
{

	Map = rhs.Map;
	SelectedSpriteIndex = rhs.SelectedSpriteIndex;
	LayerChosen = rhs.LayerChosen;

	TransparentTexture = rhs.TransparentTexture;

	OpenTilesetChoosingMenu = rhs.OpenTilesetChoosingMenu;
	DisplayTilesetChoosingMenu = rhs.DisplayTilesetChoosingMenu;

	ViewMapJson = rhs.ViewMapJson;
	DisplayViewMapJson = rhs.DisplayViewMapJson;

	OpenConstructedMapWindow = rhs.OpenConstructedMapWindow;
	DisplayConstructedMapWindow = rhs.DisplayConstructedMapWindow;

	OpenLayerNameInputWindow = rhs.OpenLayerNameInputWindow;
	DisplayLayerNameInputWindow = rhs.DisplayLayerNameInputWindow;

	RecieveTileSetSelectedEventsFlag = rhs.RecieveTileSetSelectedEventsFlag.load();

	NewLayerCounter = rhs.NewLayerCounter;

	InitializeBuffers();
	memcpy(NameBuffer, rhs.NameBuffer, 100);
	memcpy(LayerNameBuffer, rhs.LayerNameBuffer, 100);

	
}

void Ermine::NewTileMap::HelperMoveTileMapWindow(NewTileMap&& rhs)
{
	
	Map = std::move(rhs.Map);
	SelectedSpriteIndex = rhs.SelectedSpriteIndex;
	LayerChosen = rhs.LayerChosen;

	TransparentTexture = std::move(rhs.TransparentTexture);

	OpenTilesetChoosingMenu = rhs.OpenTilesetChoosingMenu;
	DisplayTilesetChoosingMenu = rhs.DisplayTilesetChoosingMenu;

	ViewMapJson = rhs.ViewMapJson;
	DisplayViewMapJson = rhs.DisplayViewMapJson;

	OpenConstructedMapWindow = rhs.OpenConstructedMapWindow;
	DisplayConstructedMapWindow = rhs.DisplayConstructedMapWindow;

	OpenLayerNameInputWindow = rhs.OpenLayerNameInputWindow;
	DisplayLayerNameInputWindow = rhs.DisplayLayerNameInputWindow;

	RecieveTileSetSelectedEventsFlag = rhs.RecieveTileSetSelectedEventsFlag.load();

	NewLayerCounter = rhs.NewLayerCounter;

	NameBuffer = rhs.NameBuffer;
	rhs.NameBuffer = nullptr;

	LayerNameBuffer = rhs.LayerNameBuffer;
	rhs.LayerNameBuffer = nullptr;

	

	Ermine::RecieverComponent::Bind(GenCallableFromMethod(&NewTileMap::RecieveTileSelectedEvents), RecieveTileSetSelectedEventsFlag,
		Ermine::EventType::TileSelectedEvent);
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