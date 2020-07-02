#include "stdafx.h"
#include "NewTileSet.h"

#include "EngineResourceHandlers/GlobalTextureCache.h"
#include "imgui.h"

namespace Ermine
{
	AddNewTileSetWindow::AddNewTileSetWindow()
	{
		InitBuffers();

		auto Cache = GlobalTextureCache::Get();
		AllTexturesToChoose = Cache->GetAllTexturesInCache();
		TextureSelected = std::vector<bool>(AllTexturesToChoose.size(), false);
	}

	AddNewTileSetWindow::~AddNewTileSetWindow()
	{
		if(NameBuffer != nullptr)
			delete[] NameBuffer;

		if(FilePathBuffer != nullptr)
			delete[] FilePathBuffer;

		if(CurrentItemForDisplayInComboBox != nullptr)
			delete[]CurrentItemForDisplayInComboBox;
	}


	void AddNewTileSetWindow::Draw()
	{
		ImGui::Begin("Add New TileSet Window : ");

		ImGui::Text("Enter TileSet Name : ");
		ImGui::SameLine();
		if (ImGui::InputTextWithHint("##EnterTileSetNameAddNewTileSetWindow", "Enter The Name Of The TileSet Here..", NameBuffer, 512))
		{
			std::string FilePath(NameBuffer);
			std::string Appendage("TileSet/");
			std::string FileType(".json");
			
			int TempSize = FilePath.size();

			FilePath = Appendage + FilePath + FileType;
			FilePath.resize(TempSize + Appendage.size() + FileType.size(),0);

			FilePathBuffer = std::strcpy(FilePathBuffer, FilePath.c_str());
		}
		ImGui::SameLine();
		ImGui::Checkbox("MakeFilePathUsingName##AddNewTileSetWindow", &MakeFilePathUsingName);

		ImGui::Text("Enter TileSet Path  : ");
		ImGui::SameLine();
		ImGui::InputTextWithHint("##EnterTileSetPathAddNewTileSetWindow", "Enter The Name Of The TileSet Here..", FilePathBuffer, 512);

		ImGui::Separator();

		ImGui::Text("TileWidth  : ");
		ImGui::SameLine();
		ImGui::InputInt("##TileWidthNewTileSet", &TileWidth);

		ImGui::Text("TileHeight : ");
		ImGui::SameLine();
		ImGui::InputInt("##TileHeightNewTileSet", &TileHeight);

		ImGui::Separator();

		ImGui::Columns(2);

		ImGui::Text("UV Inferior (Start Point) Bottom Left : ");
		ImGui::SameLine();
		ImGui::SliderFloat2("##EnterUVBottomLeftNewTileSet", UvBottomLeft, 0.0f, 1.0f);
		ImGui::NextColumn();
		ImGui::Text("X: ");
		ImGui::SameLine();
		ImGui::InputFloat("##EnterUVBottomLeftNewTileSetX", UvBottomLeft);
		ImGui::Text("Y: ");
		ImGui::SameLine();
		ImGui::InputFloat("##EnterUVBottomLeftNewTileSetY", UvBottomLeft + 1);

		ImGui::NewLine();
		ImGui::NextColumn();

		ImGui::Text("UV Superior (End Point  ) Top Right   : ");
		ImGui::SameLine();
		ImGui::SliderFloat2("##EnterUVTopRightNewTileSet", UvTopRight, 0.0f, 1.0f);
		ImGui::NextColumn();
		ImGui::Text("X: ");
		ImGui::SameLine();
		ImGui::InputFloat("##EnterUVTopRightNewTileSetX", UvTopRight);
		ImGui::Text("Y: ");
		ImGui::SameLine();
		ImGui::InputFloat("##EnterUVTopRightNewTileSetY", UvTopRight + 1);

		ImGui::Separator();

		ImGui::Columns(1);

		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(255, 255, 255)));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 168, 107)));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(ImColor(63, 122, 77)));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(ImColor(41, 171, 135)));
		if (ImGui::Button("Add TileSet##NewTileSetWindow"))
		{
			WriteTileSetToFile();
			Quit = true;
		}
		ImGui::PopStyleColor(4);
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(255, 255, 255)));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(184, 15, 10)));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(ImColor(255, 40, 0)));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(ImColor(210, 31, 60)));
		if(ImGui::Button("Cancel"))
		{
			Quit=true;
		}
		ImGui::PopStyleColor(4);

		ImGui::Separator();

		ImGui::Text("Select Texture To Be Used For The TileSet..");
		ImGui::SameLine();


		if (ImGui::BeginCombo("##SelectRequiredTextureMewTileSetWindow",CurrentItemForDisplayInComboBox))
		{
			for (int i = 0; i < AllTexturesToChoose.size(); i++)
			{
				if (ImGui::Selectable(AllTexturesToChoose[i]->GetName().c_str(), &TextureSelected[i]))
				{
					for (auto& h : TextureSelected)
						h = false;

					TextureSelected[i] = true;
					std::strcpy((char*)CurrentItemForDisplayInComboBox, AllTexturesToChoose[i]->GetName().c_str());
				}
			}
			ImGui::EndCombo();
		}

		for (int i = 0;i<AllTexturesToChoose.size();i++)
		{
			if (TextureSelected[i] == true)
			{
				ImGui::BeginChild("Texture Chosen");

				ImGui::Columns(2);

					ImGui::Image((void*)(intptr_t)AllTexturesToChoose[i]->GetTextureID(),
						ImVec2(ImGui::GetContentRegionAvail().x, AllTexturesToChoose[i]->GetHeightToMatchAspectRatio(ImGui::GetContentRegionAvail().x)),//ImVec2(AllTexturesToChoose[i]->GetWidth(), AllTexturesToChoose[i]->GetHeight()),
						ImVec2(0, 1), ImVec2(1, 0));
				
					ImGui::NextColumn();

				float ToprightMINUSBottomLeft[2] = { UvTopRight[0] - UvBottomLeft[0],UvTopRight[1] - UvBottomLeft[1] };

				

				ImVec2 CursorPos = ImGui::GetCursorPos();
				float WidthOfWidgetX = ImGui::GetContentRegionAvail().x;
				float HeightOfWidgetY = AllTexturesToChoose[i]->GetHeightToMatchAspectRatio(ImGui::GetContentRegionAvail().x);

				float TextureWidthPercentageSelected = UvTopRight[0] - UvBottomLeft[0];
				float TextureHeightPercentageSelected = UvTopRight[1] - UvBottomLeft[1];

				float ImageInMemoryWidth = TextureWidthPercentageSelected * AllTexturesToChoose[i]->GetWidth();
				float ImageInMemoryHeight = TextureHeightPercentageSelected * AllTexturesToChoose[i]->GetHeight();

				float NumberOfLinesX = ImageInMemoryWidth / TileWidth;
				float NumberOfLinesY = ImageInMemoryHeight / TileHeight;

				float EffectiveTileWidth = WidthOfWidgetX / NumberOfLinesX;
				float EffectiveTileHeight = HeightOfWidgetY / NumberOfLinesY;

				ImGui::Image((void*)(intptr_t)AllTexturesToChoose[i]->GetTextureID(),
					ImVec2(ImGui::GetContentRegionAvail().x, AllTexturesToChoose[i]->GetHeightToMatchAspectRatio(ImGui::GetContentRegionAvail().x)),//ImVec2(AllTexturesToChoose[i]->GetWidth(), AllTexturesToChoose[i]->GetHeight()),
					ImVec2(UvBottomLeft[0], (1.0f+ UvBottomLeft[1])-UvBottomLeft[1]), ImVec2(UvTopRight[0], (1.0f + UvBottomLeft[1]) - UvTopRight[1]));

				//Draw Lines In X Constant (Vertical Lines...)
				for (int i = CursorPos.x, c=0; c<NumberOfLinesX; i = i + EffectiveTileWidth,c++)//StepXEffective)
				{
					ImGui::GetForegroundDrawList()->AddLine({ ImGui::GetWindowPos().x + (float)i,  ImGui::GetWindowPos().y + (float)CursorPos.y },
						{ ImGui::GetWindowPos().x + (float)(i), ImGui::GetWindowPos().y + (float)HeightOfWidgetY },//(float)(i + HeightOfWidgetY), ImGui::GetWindowPos().y + (float)HeightOfWidgetY },//(float)CursorPos.y + (float)HeightOfWidgetY },
															ImGui::ColorConvertFloat4ToU32({ 1.0f,1.0f,1.0f,1.0f }));
				}

				//Draw Lines In Y Constant (Horizontal Lines...)
				for (int i = CursorPos.y, c = 0; c < NumberOfLinesY; i = i + EffectiveTileHeight, c++)//StepXEffective)
				{
					ImGui::GetForegroundDrawList()->AddLine({ ImGui::GetWindowPos().x + (float)CursorPos.x,ImGui::GetWindowPos().y + (float)i },
															{  ImGui::GetWindowPos().x + (float)CursorPos.x + (float)WidthOfWidgetX,ImGui::GetWindowPos().y + (float)(i) },//(float)(i + HeightOfWidgetY), ImGui::GetWindowPos().y + (float)HeightOfWidgetY },//(float)CursorPos.y + (float)HeightOfWidgetY },
						ImGui::ColorConvertFloat4ToU32({ 1.0f,1.0f,1.0f,1.0f }));
				}

				ImGui::Columns(1);
				ImGui::Separator();
				ImGui::EndChild();
			}	
		}
		ImGui::End();
	}

	void AddNewTileSetWindow::WriteTileSetToFile()
	{
		nlohmann::json TileSetFile;

		TileSetFile["TileSetName"] = NameBuffer;

		TileSetFile["TileWidth"] = TileWidth;
		TileSetFile["TileHeight"] = TileHeight;

		//TileSetFile["Texture"] = NULL;

		for (int i=0;i<TextureSelected.size();i++)
		{
			if (TextureSelected[i] == true)
			{
				TileSetFile["Texture"][AllTexturesToChoose[i]->GetFilePath().u8string().c_str()].emplace_back(UvBottomLeft[0]);
				TileSetFile["Texture"][AllTexturesToChoose[i]->GetFilePath().u8string().c_str()].emplace_back(UvBottomLeft[1]);
				TileSetFile["Texture"][AllTexturesToChoose[i]->GetFilePath().u8string().c_str()].emplace_back(UvTopRight[0]);
				TileSetFile["Texture"][AllTexturesToChoose[i]->GetFilePath().u8string().c_str()].emplace_back(UvTopRight[1]);

				TileSetFile["NumberOfTiles"] = (AllTexturesToChoose[i]->GetWidth() * AllTexturesToChoose[i]->GetHeight())/(TileHeight * TileHeight);

				break;
			}
		}

		
		
		std::ofstream FileInDisk(std::filesystem::path(this->FilePathBuffer));
		FileInDisk << TileSetFile;

		FileInDisk.close();
	}


	void Ermine::AddNewTileSetWindow::InitBuffers()
	{
		NameBuffer = new char[512]; //Create a Name Buffer To Store The Name In..
		std::memset(NameBuffer, 0, 512);

		FilePathBuffer = new char[512]; //Create a Name Buffer To Store The Name In..
		std::memset(FilePathBuffer, 0, 512);

		CurrentItemForDisplayInComboBox = new char[100];
		std::memset(CurrentItemForDisplayInComboBox, 0, 100);
		std::strcpy(CurrentItemForDisplayInComboBox, "Select From The DropDown..");
	}


	Ermine::AddNewTileSetWindow::AddNewTileSetWindow(const AddNewTileSetWindow& rhs)
	{
		CopyHelper(rhs);
	}
	AddNewTileSetWindow Ermine::AddNewTileSetWindow::operator=(const AddNewTileSetWindow& rhs)
	{
		CopyHelper(rhs);
		return *this;
	}
	
	Ermine::AddNewTileSetWindow::AddNewTileSetWindow(AddNewTileSetWindow&& rhs)
	{
		MoveHelper(std::move(rhs));
	}
	AddNewTileSetWindow Ermine::AddNewTileSetWindow::operator=(AddNewTileSetWindow&& rhs)
	{
		MoveHelper(std::move(rhs));
		return *this;
	}


	void Ermine::AddNewTileSetWindow::CopyHelper(const AddNewTileSetWindow& rhs)
	{
		AllTexturesToChoose = rhs.AllTexturesToChoose;
		InitBuffers();

		memcpy(CurrentItemForDisplayInComboBox, rhs.CurrentItemForDisplayInComboBox, 100);
		
		TextureSelected = rhs.TextureSelected;

		TileWidth = rhs.TileWidth;
		TileHeight = rhs.TileHeight;

		memcpy(UvBottomLeft, rhs.UvBottomLeft, 2 * sizeof(float));
		memcpy(UvTopRight, rhs.UvTopRight, 2 * sizeof(float));

		memcpy(NameBuffer, rhs.NameBuffer, 512);
		MakeFilePathUsingName = rhs.MakeFilePathUsingName;
		memcpy(FilePathBuffer, rhs.FilePathBuffer, 512);
	}
	void Ermine::AddNewTileSetWindow::MoveHelper(AddNewTileSetWindow&& rhs)
	{
		AllTexturesToChoose = rhs.AllTexturesToChoose;

		CurrentItemForDisplayInComboBox = rhs.CurrentItemForDisplayInComboBox;
		rhs.CurrentItemForDisplayInComboBox = nullptr;

		TextureSelected = std::move(rhs.TextureSelected);

		TileWidth = rhs.TileWidth;
		TileHeight = rhs.TileHeight;

		memcpy(UvBottomLeft, rhs.UvBottomLeft, 2 * sizeof(float));
		memcpy(UvTopRight, rhs.UvTopRight, 2 * sizeof(float));

		NameBuffer = rhs.NameBuffer;
		rhs.NameBuffer = nullptr;

		MakeFilePathUsingName = rhs.MakeFilePathUsingName;

		FilePathBuffer = rhs.FilePathBuffer;
		rhs.FilePathBuffer = nullptr;
	}
}
