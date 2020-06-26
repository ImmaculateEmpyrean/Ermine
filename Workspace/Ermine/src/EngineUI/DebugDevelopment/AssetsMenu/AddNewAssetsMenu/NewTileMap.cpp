#include "stdafx.h"
#include "NewTileMap.h"

#include "imgui.h"

Ermine::NewTileMap::NewTileMap()
{
	InitializeBuffers();

	TileDiamensions.first = 50;
	TileDiamensions.second = 50;

	/*memcpy(NameBuffer, "UntitledMap", 11);
	memcpy(NameBufferCopy, "UntitledMap", 11);

	memcpy(LayerNameBuffer, "Layer",6);*/
}

Ermine::NewTileMap::~NewTileMap()
{
	delete NameBuffer;
	NameBuffer = nullptr;
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
	//ImGui::InputTextWithHint("##GetTileMapNameNewTileMapClass", "Enter The Name of The Tile Map Here", NameBuffer, 512);
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

	ImGui::Text("Tile Width  : ");
	ImGui::SameLine();
	ImGui::InputInt("##NewTileMapGetWidth", &TileDiamensions.first, 1, 10);

	ImGui::Text("Tile Height : ");
	ImGui::SameLine();
	ImGui::InputInt("##NewTileMapGetTileHeight", &TileDiamensions.second, 1, 10);

	ImGui::Separator();

	if (ImGui::Button("View Tilesets In Use##NewTileMapTilesetsMenu"))
		OpenTilesetChoosingMenu = true;
	ImGui::SameLine();
	if (ImGui::Button("View Map XML##NewTileMapTilesetsMenu"))
		ViewMapXmL = true;
	ImGui::SameLine();

	ImGui::Separator();

	ImGui::Columns(2);
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "EDITOR :");
		ImGui::NextColumn();
		ImGui::Text("Layer Selected :");
		//ImGui::BeginCombo("##LayerSelectorComboNewTileMap", "NONE");
		//Populate Combo Here
		//ImGui::EndCombo();
		ImGui::SameLine();
		ImGui::Button("+##NewTileMapAddNewLayerButton");
		ImGui::SameLine();
		OpenLayerViewWindow = ImGui::Button("SelectedLayerView##NewTileMapAddNewLayerButton");

	ImGui::Columns(1);
	ImGui::Separator();

	if (NumberOfTiles.first > 0)
	{
		TileDiamensions.first = 50;
		TileDiamensions.second = 50;
		ImGui::SetNextWindowContentSize(ImVec2(TileDiamensions.first * NumberOfTiles.first,
			0));//TileDiamensions.second * NumberOfTiles.second));
		ImVec2 child_size = ImVec2(0, ImGui::GetFontSize() * 20.0f);
		ImGui::BeginChild("TileMap##ScrollingRegion", child_size, false, ImGuiWindowFlags_HorizontalScrollbar);
		ImGui::Columns(NumberOfTiles.first);
		for (int i = 0; i < NumberOfTiles.first * NumberOfTiles.second; i++)
		{
			static bool Sigma = false;
			ImGui::Selectable("Hanna", &Sigma, 0, { 50,50 });
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
	ImGui::Button("Cancel");
	ClearButtonColor();

	ImGui::End();
}

void Ermine::NewTileMap::InitializeBuffers()
{
	
}

void Ermine::NewTileMap::HelperCopyTileMapWindow(const NewTileMap& rhs)
{
	
}

void Ermine::NewTileMap::HelperMoveTileMapWindow(NewTileMap&& rhs)
{
	
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