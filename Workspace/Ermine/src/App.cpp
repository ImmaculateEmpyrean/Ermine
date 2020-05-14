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

#include "InputSystem/ErmineKeyCodesDefinition.h"

#include<nlohmann/json.hpp>

#include "2DPrimitives/Actor2D.h"
#include "Graphics/Renderer/Renderer2D.h"

#include "2DPrimitives/TileSet.h"


#pragma region StaticDefines

std::once_flag Ermine::App::InitializationFlag;
Ermine::App* Ermine::App::PointerToApp = nullptr;

#pragma endregion StaticDefines

Ermine::App::App(std::string AppTitle, std::pair<int, int> Diamensions)
	:
	AppTitle(AppTitle),
	Diamensions(Diamensions),
	InpInterrogator()
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

	/*const char* vertexShaderSource = "#version 330 core\n"
		"layout(location = 0) in vec3 aPos;\n"
		"layout(location = 1) in vec2 aTexCoord;\n"
		"out vec3 ourColor;\n"
		"out vec2 TexCoord;\n"
		"uniform mat4 transform;\n"
		"void main()\n"
		"{\n"
		"gl_Position =transform*vec4(aPos, 1.0);\n"
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

	static Ermine::Texture Tex = Ermine::Texture("AnoHiMitaHana.png");
	Tex.Bind(0);

	Shd.Uniformi(std::string("texture1"), 0);
	static glm::vec4 ourColor;

	/*ImGui::Begin("Color Picker");
	ImGui::ColorPicker4("Square Color",&ourColor[0]);
	ImGui::End();

	//Shd.Uniform4f(std::string("ourColor"), ourColor);
	static float RotateAngle= 90.0f;
	static glm::vec3 ScalingConstant = glm::vec3(1.0f);
	static glm::vec3 TranslateConstant = glm::vec3(0.0f);

	ImGui::Begin("Transformation Window");

	ImGui::Text("Scale : ");
	//ImGui::SameLine();
	ImGui::InputFloat("##XScaleInp", &ScalingConstant.x, 0.1, 0.5, 2);
	//ImGui::SameLine();
	ImGui::InputFloat("##YScaleInp", &ScalingConstant.y, 0.1, 0.5, 2);
	//ImGui::SameLine();
	ImGui::InputFloat("##ZScaleInp", &ScalingConstant.z, 0.1, 0.5, 2);

	ImGui::Separator();

	ImGui::Text("Rotation : ");
	ImGui::SameLine();
	ImGui::SliderFloat("##RotationAngles", &RotateAngle, 0.0f, 360.0f);
	//ImGui::SliderAngle("##RotationAngles", &RotateAngle);

	ImGui::Text("Translate : ");
	//ImGui::SameLine();
	ImGui::InputFloat("##XTranslateInp", &TranslateConstant.x, 0.1, 0.5, 2);
	//ImGui::SameLine();
	ImGui::InputFloat("##YTranslateInp", &TranslateConstant.y, 0.1, 0.5, 2);
	//ImGui::SameLine();
	ImGui::InputFloat("##ZTranslateInp", &TranslateConstant.z, 0.1, 0.5, 2);

	ImGui::End();

	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::scale(trans, ScalingConstant);
	trans = glm::rotate(trans, glm::radians(RotateAngle), glm::vec3(0.0, 0.0, 1.0));
	trans = glm::translate(trans, TranslateConstant);
	Shd.UniformMat4(std::string("transform"), trans);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);*/

	/*if (InpInterrogator.IsKeyPressed(ERMINE_KEY_A))
	{
		STDOUTDefaultLog_Error("Yay A is Pressed");
	}*/

	/*// create an empty structure (null)
	nlohmann::json j;

	// add a number that is stored as double (note the implicit conversion of j to an object)
	j["pi"] = 3.141;

	// add a Boolean that is stored as bool
	j["happy"] = true;

	// add a string that is stored as std::string
	j["name"] = "Niels";

	// add another null object by passing nullptr
	j["nothing"] = nullptr;

	// add an object inside the object
	j["answer"]["everything"] = 42;

	// add an array that is stored as std::vector (using an initializer list)
	j["list"] = { 1, 0, 2 };

	// add another object (using an initializer list of pairs)
	j["object"] = { {"currency", "USD"}, {"value", 42.99} };

	auto a =j.dump();
	STDOUTDefaultLog_Critical(a.c_str());*/
	
	/*static Texture* Tex = new Texture("AnoHiMitaHana.png");
	static Sprite* spr = new Sprite(Tex, { 0.0f,0.0f }, { 1.0f,1.0f });
	static Actor2D Act = Actor2D(spr);

	glm::mat4 Camera = glm::mat4(1.0f);
	glm::translate(Camera, glm::vec3(0.0f, 0.0f, -3.0f));

	auto ProjectionMatrix = glm::ortho<float>(-2.0f, 2.0f, -2.0f, 2.0f,-5.0f,5.0f);


	Act.Translate({ 0.001f,0.001f });
	Act.Rotate(1);
	Act.Scale({ 1.005f,1.005f });

	Renderer2D::BeginScene(Camera, ProjectionMatrix);

	Renderer2D::DrawActor2D(&Act);

	Renderer2D::EndScene();*/
	
	static Ermine::TileSet Set("TileSet/TileSetTest.json");
	
	static int c = 0;
	static int fc = 0;

	fc++;

	if (fc >= 60)
	{
		c++;
		fc = 0;
	}
	

	if (c >= Set.GetSpriteBuffer().size())
		c = 0;
	
	static Texture* Tex = new Texture("AnoHiMitaHana.png");
	static Sprite* spr = new Sprite(Tex, { 0.0f,0.0f }, { 1.0f,1.0f });
	Actor2D Act = Actor2D(Set.GetTile(c));//Set.GetTile(10));

	glm::mat4 Camera = glm::mat4(1.0f);
	glm::translate(Camera, glm::vec3(0.0f, 0.0f, -3.0f));

	auto ProjectionMatrix = glm::ortho<float>(-2.0f, 2.0f, -2.0f, 2.0f,-5.0f,5.0f);


	/*Act.Translate({ 0.001f,0.001f });
	Act.Rotate(1);
	Act.Scale({ 1.005f,1.005f });*/

	Renderer2D::BeginScene(Camera, ProjectionMatrix);

	Renderer2D::DrawActor2D(&Act);

	Renderer2D::EndScene();
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


