#include "stdafx.h"
#include "App.h"

#include "Graphics/Window/Window.h"

#ifdef ER_DEBUG_DEVELOP
#include "EngineUI/DebugDevelopment/MainWindow.h"
#endif 

#include "ExchangeStructs/ExchangeAppEvents.h"
#include "ExchangeStructs/LevelInitialization.h"
#include "ExchangeStructs/ExePointerDeclarations.h"

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

	//Start Create Initialization Structure And Call The Level Initializer So That Dll Can Have Relevant Pointers In Exe
	InitializaDLLStruct Struct;
	Struct.App = this;

	InitializeDLL(Struct);
	Checker();
	//Ended Create Initialization Structure And Call The Level Initializer So That Dll Can Have Relevant Pointers In Exe

	//Now Use The DLL To Initialize The Renderer
	InitializeScene();
}

Ermine::App::~App()
{
	delete ManagedWindow; //This Should Not Be Called When The Engine Is Still In Running State
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
	std::cout << "Current Frames Per Second:"<< fps<<std::endl;
	if (currentTime - lastTime > 1.0f)
	{
		lastTime = currentTime;
		fps = (int)framesPerSecond;
		framesPerSecond = 0;
	}
}
#pragma endregion


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

void Ermine::App::AppRoutine()
{
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
	}

	Ermine::BroadcastComponent::BroadcastEvent(std::make_unique<Ermine::OnRenderTickEvent>(UpdateCounter));
	RenderLoop(UpdateCounter);
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

//Temp Delete After USe
void Ermine::App::CallFromDll()
{
	STDOUTDefaultLog_Info("Called Method Call From DLL");
}


