#include "Dstdafx.h"
#include "EditorApp.h"

#include "EditorLayer.h"

Editor::DemirugeEditor::DemirugeEditor()
	:
	Ermine::App("EditorApp", {1920,1080})
{
	//Add The Editor Layer To The App LayerStack.. This Editor Layer Displays The Ermine Scene Editor..
	std::unique_ptr<Editor::EditorLayer> EditorLayer = std::make_unique<Editor::EditorLayer>();
	App::AddLayer(std::move(EditorLayer));
}
