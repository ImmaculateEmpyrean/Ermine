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
#include "Graphics/Renderer/MaterialSystem/Material.h"

#include "imgui.h"

#include "InputSystem/ErmineKeyCodesDefinition.h"

#include<nlohmann/json.hpp>

#include "2DPrimitives/Actor2D.h"
#include "Graphics/Renderer/Renderer2D.h"

#include "2DPrimitives/TileSet.h"
#include "2DPrimitives/TileMap.h"


#include "EngineResourceHandlers/GlobalFontCache.h"
#include "FontRenderingSystem/Font.h"

#pragma region StaticDefines

std::once_flag Ermine::App::InitializationFlag;
Ermine::App* Ermine::App::PointerToApp = nullptr;

#pragma endregion StaticDefines

Ermine::App::App(std::string AppTitle, std::pair<int, int> Diamensions)
	:
	AppTitle(AppTitle),
	Diamensions(Diamensions),
	InpInterrogator() //Scrutinize
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

	Quit = ManagedWindow->ShouldIQuit();
}

void Ermine::App::OnAttach()
{
	Obj.OnAttach(); //Maybe Take This Off If Some Prob
}

void Ermine::App::OnTick()
{
	/*static bool l = true;
	//Ermine::Material mat(std::filesystem::path("Shader/Actor2DBaseMaterial.json"));

	auto Manager = Ermine::GlobalTextureCache::Get();
	static auto Tex = Manager->GetTextureFromFile("AnoHiMitaHana.png");

	static Sprite* spr = new Sprite(Tex, { 0.0f,0.0f }, { 1.0f,1.0f });
	static std::shared_ptr<Sprite> ShSpr;

	if (l == true)
	{
		ShSpr.reset(spr);
		l = false;
	}

	static Ermine::Actor2D* Act = new Ermine::Actor2D(ShSpr);
	//Act->Translate({ 0.5f,0.5f });

	glm::mat4 Camera = glm::mat4(1.0f);
	glm::translate(Camera, glm::vec3(0.0f, 0.0f, -3.0f));

	LayerStackLayer Layer("Han");
	Layer.SubmitRenderable(Act);

	auto ProjectionMatrix = glm::ortho<float>(0.0f, ((float)Ermine::GetScreenWidth()), ((float)Ermine::GetScreenHeight()), 0.0f, -5.0f, 5.0f);//glm::ortho<float>(-1.0f, 1.0f, -1.0f, 1.0f, -5.0f, 5.0f);//glm::ortho<float>(-2.0f, 2.0f, -2.0f, 2.0f, -5.0f, 5.0f);//glm::ortho<float>(0.0f, ((float)Ermine::GetScreenWidth()), ((float)Ermine::GetScreenHeight()), 0.0f, -5.0f, 5.0f);//glm::ortho<float>(-2.0f, 2.0f, -2.0f, 2.0f, -5.0f, 5.0f);
	
	//Start Tilemap Drawing Routine//
	static Ermine::TileMap Map("TileMap/UnderConsideration.json");
	LayerStack Lay("Stigmata");
	Lay.SubmitTileMapForDrawing((const TileMap*)&Map);

	
	//Ended Tilemap Drawing Routine//

	static int ind = 10;
	static bool Coke = false;


	ImGui::Begin("Control Panel");
	ImGui::SliderInt("Get Ind", &ind, 0, 10);
	ImGui::Checkbox("Get Coke", &Coke);

	ImGui::Image((ImTextureID)Manager->GetTextureFromFile("AnoHiMitaHana.png")->GetTextureID(), { 50,50 });
	ImGui::End();


	Renderer2D::BeginScene(Camera, ProjectionMatrix);

	Renderer2D::ReplaceLayerStackWithStack(Lay);
	Renderer2D::SubmitLayer(Layer, ind);
	
	//Renderer2D::SubmitLayer(Layer);
	
	Renderer2D::EndScene();

	if (Coke == true)
	{
		Act->Translate({ 1.0f,1.0f });
		Act->Scale({ 1.02f,1.02f });
	}*/

	/*//static Ermine::TileMap Map("TileMap/TestTileMap.json");
	static Ermine::TileMap Map("TileMap/UnderConsideration.json");

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
	
	//static Texture* Tex = new Texture("AnoHiMitaHana.png");
	//Sprite* spr = new Sprite(Tex, { 0.0f,0.0f }, { 1.0f,1.0f });
	auto a = glm::vec2(0.0f, 0.0f);
	auto b = glm::vec2(1.0f, 1.0f);
	//std::shared_ptr<Sprite> spr(new Sprite(Tex, { 0.0f,0.0f }, { 1.0f,1.0f }));
	Actor2D Act = Actor2D(Set.GetTile(c));//Actor2D(spr);//Actor2D(Set.GetTile(c));//Actor2D(Set.GetTile(c));//Set.GetTile(10));
	
	glm::mat4 Camera = glm::mat4(1.0f);
	glm::translate(Camera, glm::vec3(0.0f, 0.0f, -3.0f));
	
	auto ProjectionMatrix = glm::ortho<float>(-2.0f, 2.0f, -2.0f, 2.0f, -5.0f, 5.0f);
	
	
	Renderer2D::BeginScene(Camera, ProjectionMatrix);
	
	Renderer2D::DrawTileMap(&Map);
	Renderer2D::DrawActor2D(&Act);
	
	Renderer2D::EndScene();*/

	/*//static Ermine::Font fonttest("AgencyFb.ttf",80);
	auto FontCache = GlobalFontCache::Get();
	FontCache->SetFontSize(80);
	FontCache->LoadFontFromFile(std::filesystem::path("AgencyFb.ttf"));

	auto fonttest = (*FontCache)["AgencyFb"];

	//Start Draw Freetype-gl Font Atlas..
	//static Texture* Tex = new Texture("AnoHiMitaHana.png");
	//Tex->Bind();

	Ermine::Shader AtlasShader(std::filesystem::path("Shader/Vertex/DrawTextureAtlasToScreenVertexShader.vert"),
						   std::filesystem::path("Shader/Fragment/DrawTextureAtlasToScreenFragmentShader.frag"));

	float vertices[] = {
		// positions          // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // top left 
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fonttest->FontTexture->atlas->id);//fonttest.FontAtlas->id);
	glBindVertexArray(VAO);
	AtlasShader.Bind();

	AtlasShader.Uniformi("texture1", 0);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	//Ended Draw Freetype-gl Font Atlas..*/

	static bool Coke = false;
	
	auto FontCache = GlobalFontCache::Get();
	FontCache->SetFontSize(80);
	FontCache->LoadFontFromFile(std::filesystem::path("AgencyFb.ttf"));

	auto fonttest = (*FontCache)["AgencyFb"];

	glm::vec3 Color = { 1.0f,1.0f,1.0f };

	static Ermine::Label Lab("Hello World!!", Color,fonttest);

	LayerStackLayer Layer("Han");
	Layer.SubmitRenderable(&Lab);

	glm::mat4 Camera = glm::mat4(1.0f);
	glm::translate(Camera, glm::vec3(0.0f, 0.0f, -3.0f));
	auto ProjectionMatrix = glm::ortho<float>(0.0f, ((float)Ermine::GetScreenWidth()), ((float)Ermine::GetScreenHeight()), 0.0f, -5.0f, 5.0f);

	Renderer2D::BeginScene(Camera, ProjectionMatrix);

	Renderer2D::SubmitLayer(Layer);
		
	Renderer2D::EndScene();

	ImGui::Begin("Control Panel");
	ImGui::Checkbox("Get Coke", &Coke);
	ImGui::End();

	if (Coke == true)
	{
		Lab.Translate({ 1.0f,1.0f });
		Lab.Scale({ 1.02f,1.02f });
	}
}

void Ermine::App::OnDetach()
{
	//Obj.OnDetach();
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


