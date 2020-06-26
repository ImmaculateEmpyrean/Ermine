#include "stdafx.h"
#include "TilesetViewWizard.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "imgui.h"



#include "EngineResourceHandlers/GlobalTextureCache.h"

Ermine::TilesetViewWizard::TilesetViewWizard()
{
	HelperInitializeBuffers();
	TilesetsPath = std::filesystem::path("TileSet");
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

					ImGui::ImageButton((void*)(intptr_t)Img->GetTextureID(),
						ImVec2(ImGui::GetColumnWidth(), Img->GetHeightToMatchAspectRatio(ImGui::GetColumnWidth())),
						ImVec2(1, 1), ImVec2(0, 0));

					ImGui::NextColumn();

					ImGui::Text("%s", JsonFile["TileSetName"].dump());

					ImGui::NextColumn();

					ImGui::Text("Path : %s", i.path().u8string().c_str());

					ImGui::NextColumn();
					ImGui::Separator();
				}
			}
		}
	}

	ImGui::End();

	//ImGui::ShowDemoWindow();

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
