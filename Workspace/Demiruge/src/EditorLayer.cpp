#include "Dstdafx.h"
#include "EditorLayer.h"

//TODO- The EditorLayer Really Does Not Need To Know About Opengl Stuff So Remove THEM IN EARNEST..
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "imgui.h"

namespace Editor
{
	EditorLayer::EditorLayer()
		:
		Ermine::LayerStackLayer("EditorLayer")
	{}

	void EditorLayer::Update(float Dt)
	{
		//this Function Does Not Do Anything For Now.. 
		//IMPORTANT - Donot Call Anything That Is To Be rendered In This Function.. The OpenGL Context Will not be set in the thread this function will most likely run..

		//Renderer Commands Maybe Okay In This Function.. We Should See..

	}
	void EditorLayer::Render(float Dt)
	{
		ImGui::Begin("EditorMain");
			ImGui::Text("Hello World");
		ImGui::End();
	}
}
