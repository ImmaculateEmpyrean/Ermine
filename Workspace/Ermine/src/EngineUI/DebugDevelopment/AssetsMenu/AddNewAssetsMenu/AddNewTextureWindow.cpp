#include "stdafx.h"
#include "AddNewTextureWindow.h"

#include "imgui.h"
#include "Graphics/Renderer/MaterialSystem/Texture.h"

#include "EngineResourceHandlers/GlobalTextureCache.h"

namespace Ermine
{
	AddNewTextureWindow::AddNewTextureWindow()
	{
		BufferToStoreFilePath = new char[512]; //Allocate The Buffer
		std::memset((void*)BufferToStoreFilePath, 0, 512); //clear The Buffer

		BufferToStoreName = new char[512]; //Allocate The Buffer
		std::memset((void*)BufferToStoreName, 0, 512); //clear The Buffer
	}
	AddNewTextureWindow::~AddNewTextureWindow()
	{
		delete[] BufferToStoreFilePath; //Deallocate The Buffer
		delete[] BufferToStoreName; //Deallocate The Buffer
	}


	void AddNewTextureWindow::Draw()
	{
		ImGui::Begin("Add New Texture Dialog");

		ImGui::Text("New Texture Path : ");
		ImGui::SameLine();
		ImGui::InputTextWithHint("##GetTexturePathAddNewTextureWindow",
			"Add The file extension too .png,.jpg stuff like that", (char*)BufferToStoreFilePath, 512);

		ImGui::Text("New Texture Name : ");
		ImGui::SameLine();
		ImGui::InputTextWithHint("##GetTextureNameAddNewTextureWindow",
			"Enter A Name For The Texture (Nothing About This Effects The Rendering)", (char*)BufferToStoreName, 512);

		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(255, 255, 255)));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 168, 107)));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(ImColor(63, 122, 77)));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(ImColor(41, 171, 135)));
		if (ImGui::Button("Confirm"))
		{
			std::filesystem::path EnteredPath = std::filesystem::path(BufferToStoreFilePath);
			if (std::filesystem::exists(EnteredPath))
			{
				std::filesystem::copy(EnteredPath, "Texture");
				Texture* Tex;
				if(BufferToStoreName[0] != 0)
					Tex = new Texture(EnteredPath,BufferToStoreName);
				else
					Tex = new Texture(EnteredPath);

				auto TextureCache = GlobalTextureCache::Get();
				TextureCache->PushTextureIntoCache(Tex);
				Quit = true; //There is Need Of The Window Please Do Quit..
			}
			else
			{
				ShowErrorPathIncorrectWindow = true;
			}
		}
		ImGui::PopStyleColor(4);
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(255, 255, 255)));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(184, 15, 10)));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(ImColor(255, 40, 0)));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(ImColor(210, 31, 60)));
		if (ImGui::Button("Cancel"))
		{
			Quit = true; //It Would Seem The Window HAs Outlived Its Usefullness Please Get Rid Of It..
		}
		ImGui::PopStyleColor(4);

		ImGui::End();

		if (ShowErrorPathIncorrectWindow == true)
		{
			ImGui::Begin("POP UP");
			ImGui::Text("The Path Recieved Does Not Correspond To a Valid File In the filesystem Please Check And ReEnter..");
			
			if (ImGui::Button("Ok",ImVec2(ImGui::GetWindowContentRegionWidth(),ImGui::GetContentRegionAvail().y)))
			{
				ShowErrorPathIncorrectWindow = false;
			}

			ImGui::End();
		}
	}
}
