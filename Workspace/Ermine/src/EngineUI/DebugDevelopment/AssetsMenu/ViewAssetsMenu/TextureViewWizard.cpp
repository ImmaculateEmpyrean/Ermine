#include "stdafx.h"
#include "TextureViewWizard.h"

#include "imgui.h"

namespace Ermine
{
	TextureViewWizard::TextureViewWizard()
	{
		FilterText = new char[512];

		auto TexCache = Ermine::GlobalTextureCache::Get();
		Textures = TexCache->GetCache();

	}
	TextureViewWizard::~TextureViewWizard()
	{
		delete[] FilterText;
	}

	void TextureViewWizard::Draw()
	{
		ImGui::Begin("View Texture Wizard");

		ImGui::InputTextWithHint("##FilterImageTexturesWuthNameTextureViewWizard",
			"Enter First Letters Of The Texture To Filter It Among The Pile..",
			FilterText, 512);

		ImGui::Separator();

		ImGui::Columns(3);
		
		ImGui::Text("Image");
		ImGui::NextColumn();
		ImGui::Text("Name");
		ImGui::NextColumn();
		ImGui::Text("Diamensions");

		ImGui::Separator();
		ImGui::NextColumn();

		/*for (auto i : Textures)
		{
			int DrawableWidth = ImGui::GetColumnWidth();
			ImGui::Image((void*)(intptr_t)i->GetTextureID(), ImVec2(DrawableWidth, DrawableWidth));

		}*/
		for (int i = 0; i < Textures.size(); i++)
		{
			ImGui::PushID(i);

			int DrawableWidth = ImGui::GetColumnWidth()-25;
			int DrawableHeight = ((double)DrawableWidth/(double)Textures[i]->GetWidth()) * Textures[i]->GetHeight();

			ImGui::ImageButton((void*)(intptr_t)Textures[i]->GetTextureID(), ImVec2(DrawableWidth, DrawableHeight));

			ImGui::NextColumn();

			ImGui::Text("%s", Textures[i]->GetName().c_str());

			ImGui::NextColumn();

			ImGui::Text("%d X %d", Textures[i]->GetWidth(), Textures[i]->GetHeight());

			ImGui::PopID();
			ImGui::NextColumn();
			ImGui::Separator();
		}

		

		ImGui::End();
	}
}