#include "stdafx.h"
#include "NewTileMap.h"

#include "imgui.h"

Ermine::NewTileMap::NewTileMap()
{
	InitializeBuffers();

	memcpy(NameBuffer, "UntitledMap", 11);
	memcpy(NameBufferCopy, "UntitledMap", 11);

	memcpy(LayerNameBuffer, "Layer",6);
}

Ermine::NewTileMap::~NewTileMap()
{
	delete NameBuffer;
	NameBuffer = nullptr;

	delete NameBufferCopy;
	NameBufferCopy = nullptr;

	delete PathBuffer;
	PathBuffer = nullptr;

	for (auto& i : LayerNames)
	{
		delete i;
		i = nullptr;
	}

	delete LayerNameBuffer;
	LayerNameBuffer = nullptr;
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
	std::string WindowName = "Tilemap - \"" + TileMapName + "\"";
	ImGui::Begin(WindowName.c_str());
	
	//Start Set TileMap Name Section
	ImGui::Text("TileMap Name : ");
	ImGui::SameLine();
	ImGui::InputTextWithHint("##GetTileMapNameNewTileMapClass", "Enter The Name of The Tile Map Here", NameBufferCopy, 512);
	ImGui::SameLine();
	if(ImGui::Button("Set##SetTileMapNameButtonNewTileMapClass"))	
	{
		memcpy(NameBuffer,NameBufferCopy,512);
	}
	//Ended Set TileMap Name Section

	ImGui::Text("Number Of Tiles In X : ");
	ImGui::SameLine();
	ImGui::InputInt("##NewTileMapGetNumberOfTilesX", &NumberOfTiles.first, 1, 10);

	ImGui::Text("Number Of Tiles In Y : ");
	ImGui::SameLine();
	ImGui::InputInt("##NewTileMapGetNumberOfTilesY", &NumberOfTiles.second, 1, 10);

	ImGui::Text("Tile Width : ");
	ImGui::SameLine();
	ImGui::InputInt("##NewTileMapGetWidth", &TileDiamensions.first, 1, 10);

	ImGui::Text("Tile Height : ");
	ImGui::SameLine();
	ImGui::InputInt("##NewTileMapGetTileHeight", &TileDiamensions.second, 1, 10);

	ImGui::Separator();

	if (ImGui::Button("##NewTileMapTilesetsMenu"))
		OpenTilesetChoosingMenu = true;
	ImGui::SameLine();
	if (ImGui::Button("##NewTileMapTilesetsMenu"))
		ViewMapXmL = true;
	ImGui::SameLine();

	ImGui::Separator();

	ImGui::Columns(2);
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "EDITOR : ##NewTileMapMainEditor");
		ImGui::NextColumn();
		ImGui::Text("Layer Selected : ##NewTileMapLayerSelectedBox");
		ImGui::BeginCombo("##LayerSelectorComboNewTileMap", "NONE");

		ImGui::EndCombo();
		ImGui::SameLine();
		
		ImGui::Button("+##NewTileMapAddNewLayerButton");


	ImGui::Columns(1);
	ImGui::Separator();


	//Start Section Add New Layer
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(255, 255, 255)));
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 168, 107)));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(ImColor(63, 122, 77)));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(ImColor(41, 171, 135)));
	if (ImGui::Button("AddLayer##NewTileMap"))
	{
		LayerData.emplace_back(std::vector<int>());

		char* TempName = new char[50];
		memset(TempName, 0, 50);
		memcpy(TempName, "Layer", 6);
		
		LayerNames.emplace_back(TempName);

	}
	ImGui::PopStyleColor(4);
	//Ended Section Add New Layer

	ImGui::BeginTabBar("##LayerTabBarNewTileMap");



	ImGui::EndTabBar();

	
	ImGui::End();	

	
}

void Ermine::NewTileMap::InitializeBuffers()
{
	NameBuffer = new char[512];
	NameBufferCopy = new char[512];

	LayerNameBuffer = new char[50];

	PathBuffer = new char[512];

	memset(NameBuffer, 0, 512 * sizeof(char));
	memset(NameBufferCopy, 0, 512 * sizeof(char));

	memset(LayerNameBuffer, 0, 50);

	memset(PathBuffer, 0, 512 * sizeof(char));

}

void Ermine::NewTileMap::HelperCopyTileMapWindow(const NewTileMap& rhs)
{
	LayerSelected = rhs.LayerSelected;
	LayerData = rhs.LayerData;

	InitializeBuffers();
	
	for (auto i : rhs.LayerNames)
	{
		char* TempPointer = new char[50];
		memset(TempPointer, 0, 50);
		memcpy(TempPointer, i, 50);

		LayerNames.emplace_back(TempPointer);
	}

	memcpy(LayerNameBuffer, rhs.LayerNameBuffer, 50);

	memcpy(NameBuffer, rhs.NameBuffer, 512);
	memcpy(NameBufferCopy, rhs.NameBufferCopy, 512);

	memcpy(PathBuffer, rhs.PathBuffer, 512);
}

void Ermine::NewTileMap::HelperMoveTileMapWindow(NewTileMap&& rhs)
{
	LayerSelected = rhs.LayerSelected;
	LayerData = rhs.LayerData;

	for (auto& i : rhs.LayerNames)
	{
		LayerNames.emplace_back(i);
		i = nullptr;
	}

	LayerNameBuffer = rhs.LayerNameBuffer;
	rhs.LayerNameBuffer = nullptr;

	NameBuffer = rhs.NameBuffer;
	rhs.NameBuffer = nullptr;
	NameBufferCopy = rhs.NameBufferCopy;
	rhs.NameBufferCopy = nullptr;

	PathBuffer = rhs.PathBuffer;
	rhs.PathBuffer = nullptr;
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
}

void Ermine::NewTileMap::ClearButtonColor()
{
	ImGui::PopStyleColor(4);
}

