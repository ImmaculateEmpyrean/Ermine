#include "stdafx.h"
#include "App.h"

#include "Graphics/Window/Window.h"

#ifdef ER_DEBUG_DEVELOP
#include "EngineUI/DebugDevelopment/MainWindow.h"
#endif 

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

#include "2DPrimitives/ActorFamily/Actor2D.h"
#include "2DPrimitives/ActorFamily/PhysicsActor2D.h"

#include "Graphics/Renderer/Renderer2D.h"

#include "2DPrimitives/TileSet.h"
#include "2DPrimitives/TileMap.h"


#include "EngineResourceHandlers/GlobalFontCache.h"
#include "FontRenderingSystem/Font.h"

#include "2DPrimitives/SpriteBook.h"

#include "Physics/PhysicsComponent2D.h"

//#include "NoiseGeneration/FastNoiseLite.h"
#include "NoiseGeneration/PerlinNoise.hpp"

#pragma region StaticDefines

std::once_flag Ermine::App::InitializationFlag;
Ermine::App* Ermine::App::PointerToApp = nullptr;

#pragma endregion StaticDefines

Ermine::App::App(std::string AppTitle, std::pair<int, int> Diamensions, PhysicsWorldInitializationStruct PhysicsConfig)
	:
	AppTitle(AppTitle),
	Diamensions(Diamensions),
	InpInterrogator()
{
	ManagedWindow = new Window(AppTitle, Diamensions);

	//Obj = GetAppEventsStruct();

	//Start Create Window Handler..//
	WindowHandler::GlobalWindowHandler = new WindowHandler();

#ifdef ER_DEBUG_DEVELOP
	WindowHandler::GlobalWindowHandler->SubmitWindowFront(std::make_unique<DebugMainWindow>());
#endif
	
	//Ended Create Window Handler..//
	
	//Start Setup Physics Of the engine..//
	
	//Create A Box2d Universe
	Universum = new b2World(b2Vec2(PhysicsConfig.Gravity.x, PhysicsConfig.Gravity.y));
	//Compute Box2d Physics World Bounds..
	Ermine::RecalculatePhysicsWorldBounds();

	//Ended Setup Physics of The Engine..//

	//Called The Renderer Get In Hopes That It Will Initialize The Renderer2D..
	auto Renderer = Renderer2D::Get();

	OnAttach(); //This Event Is Called Signifying That The App Is Now Attached...
	//Init();
	StoreAppHandle(this);
	OnStart();
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

	//Start Calculate Delta Time..//
	static float DeltaTimeVar = 0.0f;
	float TimeS = glfwGetTime();// -Ermine::TimeStep;
	DeltaTimeVar = TimeS - DeltaTimeVar;
	Ermine::TimeStep = Ermine::DeltaTime(DeltaTimeVar);
	DeltaTimeVar = TimeS;
	//Ended Calculate Delta Time..//
	
	Ermine::BroadcastComponent::BroadcastEvent(std::make_unique<Ermine::OnTickEvent>(DeltaTimeVar));
	OnTick();
	
	//The Physics World Has To Step So That It Is Ready For The Next Iteration..
	Universum->Step(PhysicsWorldTimestep, PhysicsVelocityIterations, PhysicsPositionIterations);

	//Update The Camera 
	auto Camera = Ermine::OrthographicCamera::Get();
	Camera->OnUpdate();

	ManagedWindow->PostNewFrameProcess();
	Quit = ManagedWindow->ShouldIQuit();
}

void Ermine::App::OnAttach()
{
	//Obj.OnAttach(); //Maybe Take This Off If Some Prob
}


static void CalculateFrameRate()
{
	static float framesPerSecond = 0.0f;
	static int fps;
	static float lastTime = 0.0f;
	float currentTime = GetTickCount() * 0.001f;
	++framesPerSecond;
	std::cout << "Current Frames Per Second:"<< fps<<std::endl;
	if (currentTime - lastTime > 1.0f)
	{
		lastTime = currentTime;
		fps = (int)framesPerSecond;
		framesPerSecond = 0;
	}
}

void Ermine::App::OnTick()
{
	static bool Once = true;

	if (Once)
	{
		Once = false;
	}

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

	static std::shared_ptr<Ermine::Actor2D> Act = Ermine::Actor2D::GenerateActor2D("AnoHiMitaHana.png");
	
	static bool Initialize = false;

	

#if 1
	
	//Layer.SubmitRenderable(&*Act);

	static int ind = 10;
	static bool Move = false;
	static bool Scale = false;
	static int Loc[2] = { 0.0f,0.0f };

	ImGui::Begin("Control Panel");
	
	ImGui::InputInt2("##GetPosition", Loc); 
	ImGui::SameLine(); 
	if (ImGui::Button("Move To Loc"))
	{
		Act->SetActorPosition(glm::vec2(Loc[0], Loc[1]));
	}

	if (ImGui::Button("<"))
	{
		Act->Translate(-1.0f,0.0f);
	}
	ImGui::SameLine();
	if (ImGui::Button(">"))
	{
		Act->Translate(1.0f, 0.0f);
	}


	ImGui::Checkbox("Get Coke", &Move);
	ImGui::Checkbox("Get Coke And Scale", &Scale);

	ImGui::Image((ImTextureID)Manager->GetTextureFromFile("AnoHiMitaHana.png")->GetTextureID(), { 50,50 });

	ImGui::Text("%d", ImGui::GetFrameCount());
	//CalculateFrameRate();
	ImGui::End();


	Renderer2D::BeginScene();

	if (Initialize == false)
	{
		LayerStackLayer Layer("Han");
		Layer.SubmitActor(Act);
		Renderer2D::SubmitLayer(std::move(Layer));
		Initialize = true;
	}


	Renderer2D::EndScene();

	if (Move == true)
	{
		Act->Translate({ 1.0f,1.0f });
		//Act->Scale({ 1.02f,1.02f });
	}

	if (Scale == true)
	{
		Act->Scale(1.1f, 1.1f);
	}

#endif
}

void Ermine::App::OnDetach()
{
	//Obj.OnDetach();
}

Ermine::App* Ermine::App::Get()
{
	std::call_once(InitializationFlag, []() {

#if defined(ER_DEBUG_DEVELOP) || defined(ER_DEBUG_SHIP)
		//Start Initializing Log Library                  Donot Initialize Logs On Release Builds..
		std::vector<std::pair<std::string, CreateLogFile>> Configuration;
		Configuration.emplace_back(std::make_pair<std::string, CreateLogFile>("Augustus", CreateLogFile::CreateLogFile));
		Log::Init(Configuration);
		//Ended Initializing Log Library
#endif

#if defined(ER_DEBUG_SHIP) || defined(ER_RELEASE_SHIP)
		PointerToApp = new App(GetGameNameString(), GetGameWindowDiamensions(), GetPhysicsWorldInitializationStruct());
#elif defined(ER_DEBUG_DEVELOP)
		PhysicsWorldInitializationStruct Phy;
		Phy.Gravity = glm::vec2(0.0f, -1.0f);		
		PointerToApp = new App("Ermine Development Environment", GetGameWindowDiamensions(),Phy);
#endif

	});

	return PointerToApp;
}


