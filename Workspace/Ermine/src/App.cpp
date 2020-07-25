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

#include <ft2build.h>
#include FT_FREETYPE_H

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

	FT_Library ft;
	if (FT_Init_FreeType(&ft))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
	else
		std::cout << "Guess Freetype Compiled Successfully" << std::endl;

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
	static bool l = true;
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

	auto ProjectionMatrix = glm::ortho<float>(-2.0f, 2.0f, -2.0f, 2.0f, -5.0f, 5.0f);//glm::ortho<float>(0.0f, ((float)Ermine::GetScreenWidth()), ((float)Ermine::GetScreenHeight()), 0.0f, -5.0f, 5.0f);//glm::ortho<float>(-2.0f, 2.0f, -2.0f, 2.0f, -5.0f, 5.0f);
	
	//Start Tilemap Drawing Routine//
	static Ermine::TileMap Map("TileMap/UnderConsideration.json");
	LayerStack Lay("Stigmata");
	Lay.SubmitTileMapForDrawing((const TileMap*)&Map);

	
	//Ended Tilemap Drawing Routine//

	static int ind = 10;

	ImGui::Begin("GEt Number");
	ImGui::SliderInt("Get Ind", &ind, 0, 10);
	ImGui::End();


	Renderer2D::BeginScene(Camera, ProjectionMatrix);

	Renderer2D::ReplaceLayerStackWithStack(Lay);
	Renderer2D::SubmitLayer(Layer, ind);
	
	//Renderer2D::SubmitLayer(Layer);
	
	Renderer2D::EndScene();

	//Act->Translate({ 0.05f,0.05f });

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


