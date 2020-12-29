#include "stdafx.h"
#include "RendererCommands.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "OpenGLErrorChecker.h"

#include "imgui.h"
#include "InputSystem/imgui_impl_opengl3.h"
#include "InputSystem/imgui_impl_glfw.h"

namespace Ermine
{
	void RendererCommands::SetClearColor(glm::vec4 ClearColor)
	{
		GLCall(glClearColor(ClearColor.x, ClearColor.y, ClearColor.z, ClearColor.w));
	}
	void RendererCommands::ClearScreen()
	{
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
	}


	void RendererCommands::ImguiNewFrame()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}
	void RendererCommands::InitializeNewFrame()
	{
		ImguiNewFrame();
		ClearScreen();
	}
	void RendererCommands::FlushFrame()
	{
		glfwPollEvents();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		auto io = &ImGui::GetIO();
		if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}

		glfwSwapBuffers(glfwGetCurrentContext());
	}

	void RendererCommands::DrawIndexed(std::shared_ptr<Ermine::VertexArray> Vao,std::shared_ptr<Ermine::Material> Mat)
	{
		//The Bind In This Context Also Constructs Said Buffers In Most Cases.. Maybe Change The Name From Bind To Something Else In The Api..
		Vao->Bind();
		Mat->Bind();

		GLCall(glDrawElements(GL_TRIANGLES,Vao->GetIndexBufferLength(),GL_UNSIGNED_INT,0));
	}	
}
