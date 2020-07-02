#include "stdafx.h"
#include "TilesetViewWizard.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "imgui.h"

#include "EngineResourceHandlers/GlobalTextureCache.h"
#include "EngineResourceHandlers/EditorDefaultStrings.h"

Ermine::TilesetViewWizard::TilesetViewWizard()
{
	HelperInitializeBuffers();
	auto Context = EditorDefaultStrings::Get();
	TilesetsPath = Context->GetValue("TilesetViewWizardTilesetsPath").value_or("Error Could Not Get A Default Path From Editor Default Strings");
	//TilesetsPath = std::filesystem::path("TileSet");
}

Ermine::TilesetViewWizard::~TilesetViewWizard()
{
	if(FilterText != nullptr)
		delete FilterText;

	if (TilesetPathInputBuffer != nullptr)
		delete TilesetPathInputBuffer;
}


Ermine::TilesetViewWizard::TilesetViewWizard(const TilesetViewWizard& rhs)
{
	HelperCopyConstructor(rhs);
}
Ermine::TilesetViewWizard Ermine::TilesetViewWizard::operator=(const TilesetViewWizard& rhs)
{
	HelperCopyConstructor(rhs);
	return *this;
}

Ermine::TilesetViewWizard::TilesetViewWizard(TilesetViewWizard&& rhs)
{
	HelperMoveConstructor(std::move(rhs));
}
Ermine::TilesetViewWizard Ermine::TilesetViewWizard::operator=(TilesetViewWizard&& rhs)
{
	HelperMoveConstructor(std::move(rhs));
	return *this;
}


void Ermine::TilesetViewWizard::Draw()
{
	ImGui::Begin("Tileset View Wizard",(bool*)0, ImGuiWindowFlags_HorizontalScrollbar);

	ImGui::Columns(3);
	ImGui::NextColumn();
	ImGui::Text("Enter Filter Text : ");
	ImGui::SameLine();
	ImGui::InputTextWithHint("##Tilesetviewwizardenterfiltertext", "Enter File Name To Thin out Options", FilterText, 100);
	ImGui::NextColumn();

	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(255, 255, 255)));
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(220, 20, 60)));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(ImColor(178, 34, 34)));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(ImColor(139, 0, 0)));
	if (ImGui::Button("Quit")) Quit = true;
	ImGui::PopStyleColor(4);
	
	ImGui::Columns(1);

	ImGui::Separator();

	ImGui::Text("Path From Which Tilesets Are Displayed : %s",TilesetsPath.u8string().c_str());
	ImGui::SameLine();
	ChangePathFromWhichTilesetsAreDisplayed = ImGui::Button("Change");

	ImGui::Separator();

	ImGui::Columns(3);

	ImGui::Text("Preview");
	ImGui::NextColumn();
	ImGui::Text("Name");
	ImGui::NextColumn();
	ImGui::Text("Path");
	ImGui::NextColumn();

	ImGui::Separator();

	//Draw Items To Be Clicked From Here
	static std::error_code er;
	std::filesystem::recursive_directory_iterator AllTilesets(TilesetsPath,er);

	for (auto i : AllTilesets)
	{
		if (HelperCheckFileExtensionForJson(i.path().u8string().c_str()))
		{
			std::ifstream RawFile(i.path().u8string().c_str());

			nlohmann::json JsonFile;
			JsonFile << RawFile;

			if (HelperCheckerIfJsonIsValid(JsonFile))
			{

				if (HelperShouldIDisplayThisString(JsonFile["TileSetName"]))
				{
					auto Cache = Ermine::GlobalTextureCache::Get();
					auto items = JsonFile["Texture"].items();
					auto Img = Cache->GetTextureFromFile(items.begin().key());

					if (ImGui::ImageButton((void*)(intptr_t)Img->GetTextureID(),
						ImVec2(ImGui::GetColumnWidth(), Img->GetHeightToMatchAspectRatio(ImGui::GetColumnWidth())),
						ImVec2(1, 1), ImVec2(0, 0)))
					{
						auto Handler = Ermine::WindowHandler::Get();
						Handler->SubmitWindowFront(std::make_unique<Ermine::TilesetViewer>(Ermine::TilesetViewer(i.path())));
					}

					ImGui::NextColumn();

					std::string temp = JsonFile["TileSetName"].dump();
					temp.erase(temp.begin());
					temp.erase(temp.end() - 1);
					ImGui::Text("%s", temp.c_str());

					ImGui::NextColumn();

					ImGui::Text("Path : %s", i.path().u8string().c_str());

					ImGui::NextColumn();
					ImGui::Separator();
				}
			}
		}
	}

	ImGui::End();


	if (ChangePathFromWhichTilesetsAreDisplayed)
	{
		ChangePathFromWhichTilesetsAreDisplayed = false;
		DisplayChangePathFromWhichTilesetsAreDisplayedWindow = true;
	}

	if (DisplayChangePathFromWhichTilesetsAreDisplayedWindow)
	{
		//Draw The Change Path Window
		ImGui::Begin("Change Path On Where To Look For The Tilesets",(bool*)0,ImGuiWindowFlags_AlwaysAutoResize);

		ImGui::Text("New Path : ");
		ImGui::InputTextWithHint("##NewPAthToLookAtTilesetviewwizard", "Enter New PAth From Which To Search For Tilesets",
			TilesetPathInputBuffer, 100);

		static bool MakeDefault = false;
		ImGui::Text("Make This Path The Default Path : ");
		ImGui::SameLine();
		ImGui::Checkbox("##MakeDefaultPathThisTilesetViewWizard", &MakeDefault);

		if (ImGui::Button("OK"))
		{
			if (MakeDefault == true)
			{
				auto Context = Ermine::EditorDefaultStrings::Get();
				Context->SubmitChanges("TilesetViewWizardTilesetsPath", std::string(TilesetPathInputBuffer));
			}
			
			TilesetsPath = std::filesystem::path(TilesetPathInputBuffer);
			memset(TilesetPathInputBuffer, 0, 100);
			DisplayChangePathFromWhichTilesetsAreDisplayedWindow = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
		{
			memset(TilesetPathInputBuffer, 0, 100);
			DisplayChangePathFromWhichTilesetsAreDisplayedWindow = false;
		}

		ImGui::End();
	}
}


bool Ermine::TilesetViewWizard::HelperCheckFileExtensionForJson(std::string FileName)
{
	std::string ExtensionStorage;

	for (auto i = FileName.rbegin(); i != FileName.rend(); i++)
	{
		if (*i == '.')
			break;
		ExtensionStorage = *i + ExtensionStorage;
	}

	if (ExtensionStorage == std::string("json"))
		return true;
	else
		return false;
}

bool Ermine::TilesetViewWizard::HelperCheckerIfJsonIsValid(nlohmann::json JsonFile)
{
	bool Flag = false;

	for (auto i : JsonFile.items())
	{
		if (i.key() == std::string("TileSetName"))
		{
			Flag = true;
			break;
		}
	}

	return Flag;
}

bool Ermine::TilesetViewWizard::HelperShouldIDisplayThisString(std::string StringToDisplay)
{
	//std::string FilterTextString(FilterText);
	size_t found = StringToDisplay.find(FilterText);

	if (found != std::string::npos)
		return true;
	else
		return false;
}


void Ermine::TilesetViewWizard::HelperInitializeBuffers()
{
	FilterText = new char[100];
	memset(FilterText, 0, 100);

	TilesetPathInputBuffer = new char[100];
	memset(TilesetPathInputBuffer, 0, 100);
}

void Ermine::TilesetViewWizard::HelperCopyConstructor(const TilesetViewWizard& rhs)
{
	HelperInitializeBuffers();

	memcpy(FilterText, rhs.FilterText, 100);
	memcpy(TilesetPathInputBuffer, rhs.TilesetPathInputBuffer, 100);

	TilesetsPath = rhs.TilesetsPath;

	ChangePathFromWhichTilesetsAreDisplayed = rhs.ChangePathFromWhichTilesetsAreDisplayed;
	DisplayChangePathFromWhichTilesetsAreDisplayedWindow = rhs.DisplayChangePathFromWhichTilesetsAreDisplayedWindow;
}

void Ermine::TilesetViewWizard::HelperMoveConstructor(TilesetViewWizard&& rhs)
{
	FilterText = rhs.FilterText;
	rhs.FilterText = nullptr;

	TilesetPathInputBuffer = rhs.TilesetPathInputBuffer;
	rhs.TilesetPathInputBuffer = nullptr;

	TilesetsPath = rhs.TilesetsPath;

	ChangePathFromWhichTilesetsAreDisplayed = rhs.ChangePathFromWhichTilesetsAreDisplayed;
	DisplayChangePathFromWhichTilesetsAreDisplayedWindow = rhs.DisplayChangePathFromWhichTilesetsAreDisplayedWindow;
}
