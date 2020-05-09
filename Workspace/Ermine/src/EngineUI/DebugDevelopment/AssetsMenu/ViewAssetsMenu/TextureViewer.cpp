#include "stdafx.h"
#include "TextureViewer.h"

#include "imgui.h"

namespace Ermine
{
	TextureViewer::TextureViewer(Texture* Tex)
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

		ImGui::Image((void*)(intptr_t)Tex->GetTextureID(), ImVec2(Tex->GetWidth(), Tex->GetHeight()));


		ImGui::End();
	}
}
