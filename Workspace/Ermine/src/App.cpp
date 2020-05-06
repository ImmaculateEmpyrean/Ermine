#include "stdafx.h"
#include "App.h"

#include "Graphics/Window/Window.h"

#include "EngineUI/DebugDevelopment/MainWindow.h"

#include "ExchangeStructs/ExchangeAppEvents.h"

#include "vec2.hpp"
#include "vec4.hpp"
#include "glm.hpp"

#include "Graphics/Renderer/RendererPrimitives/VertexArray.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Graphics/Renderer/MaterialSystem/Shader.h"
#include "Graphics/Renderer/MaterialSystem/Texture.h"

#include "imgui.h"

#pragma region StaticDefines

std::once_flag Ermine::App::InitializationFlag;
Ermine::App* Ermine::App::PointerToApp = nullptr;

#pragma endregion StaticDefines

Ermine::App::App(std::string AppTitle, std::pair<int, int> Diamensions)
	:
	AppTitle(AppTitle),
	Diamensions(Diamensions)
{
	ManagedWindow = new Window(AppTitle, Diamensions);

	Obj = GetAppEventsStruct();

	//Start Create Window Handler..//
	WindowHandler::GlobalWindowHandler = new WindowHandler();
	WindowHandler::GlobalWindowHandler->SubmitWindowFront(std::make_unique<DebugMainWindow>());
	//Ended Create Window Handler..//

	OnAttach(); //This Event Is Called Signifying That The App Is Now Attached...
}

Ermine::App::~App()
{
	delete ManagedWindow; //This Should Not Be Called When The Engine Is Still In Running State
	delete WindowHandler::GlobalWindowHandler;

	OnDetach(); //The Event Is Called...
}

void Ermine::App::NextFrame()
{
	ManagedWindow->PreNewFrameProcess();
	WindowHandler::GlobalWindowHandler->UpdateDraw();
	OnTick();
	ManagedWindow->PostNewFrameProcess();
}

void Ermine::App::OnAttach()
{
	Obj.OnAttach();




}

void Ermine::App::OnTick()
{
	/*const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos, 1.0);\n"
		"}\0";

	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"uniform vec4 ourColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = ourColor;\n"
		"}\n\0";*/

	const char* vertexShaderSource = "#version 330 core\n"
		"layout(location = 0) in vec3 aPos;\n"
		"layout(location = 1) in vec2 aTexCoord;\n"
		"out vec3 ourColor;\n"
		"out vec2 TexCoord;\n"
		"void main()\n"
		"{\n"
		"gl_Position = vec4(aPos, 1.0);\n"
		"TexCoord = vec2(aTexCoord.x, aTexCoord.y);\n"
		"}";

	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"in vec2 TexCoord;\n"
		"uniform sampler2D texture1;\n"
		"void main()\n"
		"{\n"
		"FragColor = texture(texture1, TexCoord);\n"
		"}\n";

	Ermine::Shader Shd = Ermine::Shader(std::string(vertexShaderSource),std::string(fragmentShaderSource));
	Shd.Bind();

	std::vector<float> VertexBuffer = { 0.5f,  0.5f, 0.0f, 1.0,1.0,  // top right
	                                    0.5f, -0.5f, 0.0f, 0.0,1.0,  // bottom right
	                                   -0.5f, -0.5f, 0.0f, 0.0,0.0, // bottom left
	                                   -0.5f,  0.5f, 0.0f, 1.0,0.0// top left 
									  };

	std::vector<uint32_t> IndexBuffer = {
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	Ermine::VertexArray Vao(VertexBuffer, IndexBuffer);
	Vao.SetVertexAttribArray(std::vector<VertexAttribPointerSpecification>({ { 3,GL_FLOAT,false },
																			 { 2,GL_FLOAT,false} 
																			}));//{{3,GL_FLOAT,false}});

	Ermine::Texture Tex = Ermine::Texture("AnoHiMitaHana.png");
	Tex.Bind(0);

	Shd.Uniformi(std::string("texture1"), 0);
	static glm::vec4 ourColor;

	ImGui::Begin("Color Picker");
	ImGui::ColorPicker4("Square Color",&ourColor[0]);
	ImGui::End();

	//Shd.Uniform4f(std::string("ourColor"), ourColor);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Ermine::App::OnDetach()
{
	Obj.OnDetach();
}

Ermine::App* Ermine::App::Get()
{
	std::call_once(InitializationFlag, []() {

#if defined(ER_DEBUG_DEVELOP) || defined(ER_DEBUG_SHIP)
		//Donot Initialize Logs On Release Builds..
		//Start Initializing Log Library
		std::vector<std::pair<std::string, CreateLogFile>> Configuration;
		Configuration.emplace_back(std::make_pair<std::string, CreateLogFile>("Augustus", CreateLogFile::CreateLogFile));
		Log::Init(Configuration);
		//Ended Initializing Log Library
#endif

#if defined(ER_DEBUG_SHIP) || defined(ER_RELEASE_SHIP)
		PointerToApp = new App(GetGameNameString(), GetGameWindowDiamensions());
#elif defined(ER_DEBUG_DEVELOP)
		PointerToApp = new App("Ermine Development Environment", GetGameWindowDiamensions());
#endif

	});

	return PointerToApp;
}


