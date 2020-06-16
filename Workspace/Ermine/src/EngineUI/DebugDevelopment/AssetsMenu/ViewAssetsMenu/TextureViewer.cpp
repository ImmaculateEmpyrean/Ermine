#include "stdafx.h"
#include "TextureViewer.h"

#include "imgui.h"

namespace Ermine
{
	TextureViewer::TextureViewer(std::shared_ptr<Texture> Tex)
		:
		Tex(Tex)
	{
		WindowName = std::string(Tex->GetName());
		WindowName = WindowName + " Viewer";
	}


	void TextureViewer::Draw()
	{		
		ImGui::Begin(WindowName.c_str());

		ImGui::Text("Texture Diamensions : ");
		ImGui::SameLine();
		ImGui::Text("%d X %d", Tex->GetWidth(), Tex->GetHeight());
		ImGui::SameLine();
		ImGui::Text(" (width x height)");

		ImGui::Separator();

		ImGui::Image((void*)(intptr_t)Tex->GetTextureID(), ImVec2(Tex->GetWidth(), Tex->GetHeight()),
			ImVec2(0, 1), ImVec2(1, 0)); //Flipped Vertically Because stbi already flips it once for opengl

		ImGui::Separator();

		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(255, 255, 255)));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 168, 107)));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(ImColor(63, 122, 77)));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(ImColor(41, 171, 135)));
		if (ImGui::Button("Confirm",ImVec2(ImGui::GetWindowContentRegionWidth(),ImGui::GetContentRegionAvail().y)))
		{
			Quit = true;
		}
		ImGui::PopStyleColor(4);

		ImGui::End();
	}
}
