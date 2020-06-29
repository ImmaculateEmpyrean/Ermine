#include "stdafx.h"
#include "TilesetViewer.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "imgui.h"

#include "EventSystem/Components/BroadcastComponent.h"

namespace Ermine
{
	TilesetViewer::TilesetViewer(std::filesystem::path TileSetPath)
		:
		Set(TileSetPath)
	{
		Selected.resize(Set.GetNumberOfSpritesInTileSet(), 0);
		
		HelperCalculateNumberOfRowsAndColumns();
	}


	void TilesetViewer::Draw()
	{
																										   //This 5.0f * x is given so that adequate gap is given to all tiles
		ImGui::SetNextWindowContentSize(ImVec2(NumberOfRowsAndColumns.second *Set.GetTileWidth() + (5.0f * NumberOfRowsAndColumns.second), 500.0f));

		ImGui::Begin(Set.GetName().c_str());

		ImGui::Text("TileWidth : %d TileHeight : %d", Set.GetTileWidth(), Set.GetTileHeight());
		ImGui::Text("Number Of Tiles in the set: %d", Set.GetNumberOfSpritesInTileSet());

		ImGui::Separator();

		float ContentRegionWidth = ImGui::GetContentRegionAvailWidth();
		int NumberOfColumns = ContentRegionWidth / (Set.GetTileWidth());

		if (NumberOfColumns > Set.GetNumberOfSpritesInTileSet())
			NumberOfColumns = Set.GetNumberOfSpritesInTileSet();

		ImGui::Columns(NumberOfColumns,(const char*)0,false);

		int c = 0;
		for (auto i : Set.GetSpriteBuffer())
		{
			ImGui::PushID(c);
			
			auto cursorpos = ImGui::GetCursorPos();
			int* test = &(Selected[c]);
			if (ImGui::Selectable("##SelectableTilesetViewer", (bool*)test, 0, ImVec2(Set.GetTileWidth(), Set.GetTileHeight()))) //Finish This Next Sitting..
			{
				//Start Reset And Select What U Want
				//First Turn Off All Selectables
				for (int k = 0; k < Selected.size(); k++)
					Selected[k] = 0;
				//Then Select What U Actually Selected
				Selected[c] = 1;
				//Ended Reset And Select What U Want

				//Start Generate And Broadcast Event
				std::unique_ptr<Ermine::TileSelectedEvent> EventObj = Ermine::TileSelectedEvent::GenerateEvent(Set.GetFilePath(),c + 1); //c+1 because tilesets start from 1 not zero :>
				Ermine::BroadcastComponent::BroadcastEvent(std::move(EventObj));
				//Ended Generate And Broadcast Event
			}
			ImGui::SetCursorPos(cursorpos);
			ImGui::Image((void*)(intptr_t)i->GetTexture()->GetTextureID(), ImVec2(Set.GetTileWidth(), Set.GetTileHeight()),
						 ImVec2(i->GetTopRightUV().x, i->GetTopRightUV().y), ImVec2(i->GetBottomLeftUV().x, i->GetBottomLeftUV().y));
			
			ImGui::PopID();
			c++;
			ImGui::NextColumn();
		}

		ImGui::Separator();

		if (ImGui::Button("Quit"))
			Quit = true;

		ImGui::End();
	}


	void TilesetViewer::HelperCalculateNumberOfRowsAndColumns()
	{
		std::vector<int> Storage;

		int c = 2;
		while (c < Set.GetNumberOfSpritesInTileSet())
		{
			if (not(Set.GetNumberOfSpritesInTileSet() % c))
				Storage.emplace_back(c);
			c++;
		}

		c = 2;
		int NewNum = Set.GetNumberOfSpritesInTileSet() / Storage.back();
		NumberOfRowsAndColumns.first = Storage.back();
		Storage.clear();

		if (NewNum != 2)
		{
			while (c < NewNum)
			{
				if (not(NewNum % c))
					Storage.emplace_back(c);
				c++;
			}
			NumberOfRowsAndColumns.second = Storage.back();
		}
		else
			NumberOfRowsAndColumns.second = 2;
	}
}
