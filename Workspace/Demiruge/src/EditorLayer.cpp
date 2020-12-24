#include "Dstdafx.h"
#include "EditorLayer.h"

#include "Core.h"

#include "Ermine.h"

//TODO- The EditorLayer Really Does Not Need To Know About Opengl Stuff So Remove THEM IN EARNEST..
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "imgui.h"

namespace Editor
{
	Ermine::Entity* entity;

	EditorLayer::EditorLayer()
		:
		Ermine::LayerStackLayer("EditorLayer")
	{
		entity = new Ermine::Entity(Ermine::H_App->GetScene());

		std::string TestString = "TestStringTag";
		entity->AddComponent<Ermine::TagComponent>(TestString);
	}

	void EditorLayer::Update(float Dt)
	{
		//this Function Does Not Do Anything For Now.. 
		//IMPORTANT - Donot Call Anything That Is To Be rendered In This Function.. The OpenGL Context Will not be set in the thread this function will most likely run..

		//Renderer Commands Maybe Okay In This Function.. We Should See..

		static int count = 0;

		auto TagComponent = entity->GetComponent<Ermine::TagComponent>();
		std::cout << "Tag Component : " << TagComponent.Tag;
	}
	void EditorLayer::Render(float Dt)
	{
		ImGui::Begin("EditorMain");
			ImGui::Text("Hello World");
		ImGui::End();
	}
}
