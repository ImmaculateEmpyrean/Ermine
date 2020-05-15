#include "stdafx.h"
#include "NewTileSet.h"

#include "EngineResourceHandlers/GlobalTextureCache.h"
#include "imgui.h"

namespace Ermine
{
	AddNewTileSetWindow::AddNewTileSetWindow()
	{
		NameBuffer = new char[512]; //Create a Name Buffer To Store The NAme In..
		std::memset(NameBuffer, 0, 100);

		auto Cache = GlobalTextureCache::Get();
		AllTexturesToChoose = Cache->GetAllTexturesInCache();
		TextureSelected = std::vector<bool>(AllTexturesToChoose.size(), false);

		CurrentItemForDisplayInComboBox = new char[100];
		std::memset(CurrentItemForDisplayInComboBox, 0, 100);
		std::strcpy(CurrentItemForDisplayInComboBox, "Select From The DropDown..");
	}

	AddNewTileSetWindow::~AddNewTileSetWindow()
	{
		delete[] NameBuffer;
		delete[]CurrentItemForDisplayInComboBox;
	}


	void AddNewTileSetWindow::Draw()
	{
		ImGui::Begin("Add New TileSet Window : ");

		


		ImGui::Text("Enter TileSet Name : ");
		ImGui::SameLine();
		ImGui::InputTextWithHint("##EnterTileSetNameAddNewTileSetWindow", "Enter The Name Of The TileSet Here..", NameBuffer, 100);

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
					//CurrentItemForDisplayInComboBox = AllTexturesToChoose[i]->GetName().c_str();
				}
			}
			ImGui::EndCombo();
		}

		for (int i = 0;i<AllTexturesToChoose.size();i++) //: AllTexturesToChoose)
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


				ImGui::Image((void*)(intptr_t)AllTexturesToChoose[i]->GetTextureID(),
					ImVec2(ImGui::GetContentRegionAvail().x, AllTexturesToChoose[i]->GetHeightToMatchAspectRatio(ImGui::GetContentRegionAvail().x)),//ImVec2(AllTexturesToChoose[i]->GetWidth(), AllTexturesToChoose[i]->GetHeight()),
					ImVec2(UvBottomLeft[0], (1.0f+ UvBottomLeft[1])-UvBottomLeft[1]), ImVec2(UvTopRight[0], (1.0f + UvBottomLeft[1]) - UvTopRight[1]));

					ImGui::Columns(1);

					ImGui::EndChild();
			}
		}
		ImGui::End();
	}
}
