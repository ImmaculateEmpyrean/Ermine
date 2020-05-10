#include "stdafx.h"
#include "TextureViewWizard.h"

#include "imgui.h"

namespace Ermine
{
	TextureViewWizard::TextureViewWizard()
	{
		FilterText = new char[512];

		auto TexCache = Ermine::GlobalTextureCache::Get();

		for (auto i : TexCache->InternalBuffer)
			Textures.emplace_back(i.second);
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

		for (int i = 0; i < Textures.size(); i++)
		{
			if (HelperShouldIDisplayThisImage(Textures[i]))
			{
				ImGui::PushID(i);

				int DrawableWidth = ImGui::GetColumnWidth() - 25;
				int DrawableHeight = ((double)DrawableWidth / (double)Textures[i]->GetWidth()) * Textures[i]->GetHeight();

				if (ImGui::ImageButton((void*)(intptr_t)Textures[i]->GetTextureID(), ImVec2(DrawableWidth, DrawableHeight),
					ImVec2(0, 1), ImVec2(1, 0))) //Flipped Vertically Because stbi already flips it once for opengl
				{
					auto Handler = Ermine::WindowHandler::Get();
					Handler->SubmitWindowFront(std::make_unique<Ermine::TextureViewer>(Ermine::TextureViewer(Textures[i])));
				}

				ImGui::NextColumn();

				ImGui::Text("%s", Textures[i]->GetName().c_str());

				ImGui::NextColumn();

				ImGui::Text("%d X %d", Textures[i]->GetWidth(), Textures[i]->GetHeight());

				ImGui::PopID();
				ImGui::NextColumn();
				ImGui::Separator();
			}
			else continue;
		}

		

		ImGui::End();
	}
	bool TextureViewWizard::HelperShouldIDisplayThisImage(Texture* Tex)
	{
		//std::string FilterTextString(FilterText);
		size_t found = Tex->GetName().find(FilterText);

		if (found != std::string::npos)
			return true;
		else
			return false;
	}
}