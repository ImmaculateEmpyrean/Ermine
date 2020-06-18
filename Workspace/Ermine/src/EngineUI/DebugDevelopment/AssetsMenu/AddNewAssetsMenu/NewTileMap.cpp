#include "stdafx.h"
#include "NewTileMap.h"

#include "imgui.h"

Ermine::NewTileMap::NewTileMap()
{
	NameBuffer = new char[512];
	PathBuffer = new char[512];

	memset(NameBuffer, 0, 512 * sizeof(char));
	memset(PathBuffer, 0, 512 * sizeof(char));

	memcpy(NameBuffer, "UntitledMap", 11);
}

Ermine::NewTileMap::~NewTileMap()
{
	delete NameBuffer;
	NameBuffer = nullptr;

	delete PathBuffer;
	PathBuffer = nullptr;
}

void Ermine::NewTileMap::Draw()
{
	std::string WindowName(NameBuffer);
	WindowName = WindowName + " TileMap Editor";

	ImGui::Begin(WindowName.c_str());
		
	ImGui::End();	
}
