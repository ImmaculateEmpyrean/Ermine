#include "stdafx.h"
#include "App.h"

#include "Graphics/Window/Window.h"

#ifdef ER_DEBUG_DEVELOP
#include "EngineUI/DebugDevelopment/MainWindow.h"
#endif 

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

#include "LayerSystem/LayerStackLayer.h"

//#include "NoiseGeneration/FastNoiseLite.h"
#include "NoiseGeneration/PerlinNoise.hpp"

Ermine::App::App(std::string AppTitle, std::pair<int, int> Diamensions, std::filesystem::path LevelPath)
	:
	AppTitle(AppTitle),
	Diamensions(Diamensions),
	BeginLevel(LevelPath)
{
#if defined(ER_DEBUG_DEVELOP) || defined(ER_DEBUG_SHIP)
	//Start Initializing Log Library                  Donot Initialize Logs On Release Builds..
	std::vector<std::pair<std::string, CreateLogFile>> Configuration;
	Configuration.emplace_back(std::make_pair<std::string, CreateLogFile>("Augustus", CreateLogFile::CreateLogFile));
	Log::Init(Configuration);
	//Ended Initializing Log Library
#endif

	ManagedWindow = new Window(AppTitle, Diamensions);

	//Ermine Only Supports Gui From UI From ImGui For The Forseeble Future.. Hence Game Windows Are Also Probably Gonna Be Drawn Using Imgui..
	WindowHandler::GlobalWindowHandler = new WindowHandler();
#ifdef ER_DEBUG_DEVELOP
	//Only In Debug Develop Mode Do We Need To Draw The Development Tools Interface.. Maybe QT Editor Is Coming Soon..
	WindowHandler::GlobalWindowHandler->SubmitWindowFront(std::make_unique<DebugMainWindow>());
#endif
	
	//Start Setup Physics Of the engine.. With Default Earthlike Gravity..//
	Universum = new b2World(b2Vec2({BeginLevel.GetGravityVector().x,BeginLevel.GetGravityVector().y}));
	Ermine::RecalculatePhysicsWorldBounds(); //What This Func Does IS Suspect For Now..

	//Initialize The Only Renderer In Ermine For Now..
	auto Renderer = Renderer2D::Get();

	//Initialize Event Station
	auto Station = Ermine::EventBroadcastStation::GetStation();

	BeginLevel.LoadLevel();

	Ermine::LayerStackLayer Layer("DefaultLayer");
	std::vector<std::shared_ptr<Ermine::Actor2DBase>> Buf = BeginLevel.GetActors();
	for(std::shared_ptr<Ermine::Actor2DBase> i : Buf)
		Layer.SubmitActor(i);

	//This Works..
	//For Some Reason I Cant Construct In the Callback Function..
	std::shared_ptr<Ermine::Actor2D> Act2 = Ermine::Actor2D::GenerateActor2D("Texture/ErmineNullTexture.png", { 500.0f,500.0f });
	Layer.SubmitActor(Act2);

	Renderer->SubmitLayer(std::move(Layer));
}

Ermine::App::~App()
{
	//App Must Close All The Things it Handles..

	//Deallocate Box2D
	delete Universum;

	//Destroy Managed Window..
	delete ManagedWindow; 

	//Finally Remove All Gui Being Drawn..
	delete WindowHandler::GlobalWindowHandler;
}

#pragma region HelperFunctions
static void CalculateFrameRate()
{
	static float framesPerSecond = 0.0f;
	static int fps;
	static float lastTime = 0.0f;
	float currentTime = GetTickCount() * 0.001f;
	++framesPerSecond;

	STDOUTDefaultLog_Info(std::to_string(fps));

	if (currentTime - lastTime > 1.0f)
	{
		lastTime = currentTime;
		fps = (int)framesPerSecond;
		framesPerSecond = 0;
	}
}
#pragma endregion


void Ermine::App::AppRoutine()
{
	CalculateFrameRate();

	//Start Calculate Delta Time..//
	static float DeltaTimeVar = 0.0f;
	float TimeS = glfwGetTime();// -Ermine::TimeStep;
	DeltaTimeVar = TimeS - DeltaTimeVar;
	Ermine::TimeStep = Ermine::DeltaTime(DeltaTimeVar);
	DeltaTimeVar = TimeS;
	//Ended Calculate Delta Time..//

	static double UpdateCounter = 0.0;
	UpdateCounter = UpdateCounter + TimeStep.GetSeconds();
	
	if (UpdateCounter >= 0.04) //  1.0f/25.0f = 0.0f Update Cycle Is Locked To 25 fps..
	{
		UpdateCounter = 0.0;
		UpdateLoop();
		Ermine::BroadcastComponent::BroadcastEvent(std::make_unique<Ermine::OnUpdateTickEvent>());
		STDOUTDefaultLog_Critical("Update Tick Broadcasted");
	}
	else
	{
		Ermine::BroadcastComponent::BroadcastEvent(std::make_unique<Ermine::OnRenderTickEvent>(UpdateCounter));
		STDOUTDefaultLog_Critical("Render Tick Broadcasted");
		RenderLoop(UpdateCounter);
	}
}

void Ermine::App::UpdateLoop()
{
	//Update Loop Is Called A 

	Universum->Step(1.0f/25.0f , PhysicsVelocityIterations, PhysicsPositionIterations); //Since Update Cycle Is Locked To 25 fps I Have Decided To Hard Code 25 Value..

	//Update The Camera 
	auto Camera = Ermine::OrthographicCamera::Get();
	Camera->OnUpdate();

	Quit = ManagedWindow->ShouldIQuit();
}

void Ermine::App::RenderLoop(float DeltaTime)
{
	ManagedWindow->PreNewFrameProcess();
	WindowHandler::GlobalWindowHandler->UpdateDraw();

	Renderer2D::Draw();

	ManagedWindow->PostNewFrameProcess();
}
