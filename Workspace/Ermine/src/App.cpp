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

	Obj = GetAppEventsStruct();

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
	
	OnTick();
	
	//The Physics World Has To Step So That It Is Ready For The Next Iteration..
	Universum->Step(PhysicsWorldTimestep, PhysicsVelocityIterations, PhysicsPositionIterations);

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
	
	auto Manager = Ermine::GlobalTextureCache::Get();
	static auto Tex = Manager->GetTextureFromFile("Texture/St.MichaelHeraldry.png");

	static Sprite* spr = new Sprite(Tex, { 0.0f,0.0f }, { 1.0f,1.0f });
	static std::shared_ptr<Sprite> ShSpr;

	if (l == true)
	{
		ShSpr.reset(spr);
		l = false;
	}

	static Ermine::Actor2D* Act = new Ermine::Actor2D(ShSpr);
	//Act->Translate({ 0.5f,0.5f });

	LayerStackLayer Layer("Han");
	Layer.SubmitRenderable(Act);

	//Ended Tilemap Drawing Routine//

	static int ind = 10;
	static bool SuiteOn = false;
	static bool TranslateOn = false;
	static bool ScaleOn = false;
	static bool RotateOn = false;

	static float Position[2] = { 0.0f,0.0f };
	static float Scale[2] = { 1.0f,1.0f };
	static float rotation = 0.0f;

	ImGui::Begin("Control Panel");
	ImGui::Checkbox("Test Suite", &SuiteOn);
	ImGui::Checkbox("Test Suite Only Translation", &TranslateOn);
	ImGui::Checkbox("Test Suite Only Scale", &ScaleOn);
	ImGui::Checkbox("Test Suite Only Rotation", &RotateOn);

	ImGui::Text("Set Position Of The Heraldic Shield : ");
	ImGui::SameLine();
	ImGui::InputFloat2("##Heraldry Position", Position);
	ImGui::SameLine();
	if(ImGui::Button("Apply##Pos"))
	{
		Act->SetPosition(Position[0], Position[1]);
	}

	ImGui::Text("Set Scale Of The Heraldic Shield : ");
	ImGui::SameLine();
	ImGui::InputFloat2("##Heraldry Rotation", Scale);
	ImGui::SameLine();
	if (ImGui::Button("Apply##Scale"))
	{
		Act->SetScale(Scale[0], Scale[1]);
	}

	ImGui::Text("Set Rotation Of The Heraldic Shield : ");
	ImGui::SameLine();
	ImGui::InputFloat("##Heraldry Rotation", &rotation);
	ImGui::SameLine();
	if (ImGui::Button("Apply##Rotate"))
	{
		Act->SetRotation(rotation);
	}

	ImGui::Image((ImTextureID)Manager->GetTextureFromFile("Texture/St.MichaelHeraldry.png")->GetTextureID(), { 50,50 });
	ImGui::End();


	Renderer2D::BeginScene();
	Renderer2D::SubmitLayer(Layer);
	Renderer2D::EndScene();

	if (SuiteOn == true)
	{
		Act->Translate({ 5.0f,5.0f });
		Act->Scale(1.005f, 1.005f);
		Act->Rotate(1.0f);
	}
	if (TranslateOn == true)
	{
		Act->Translate({ 5.0f,5.0f });
	}
	if (ScaleOn == true)
	{
		Act->Scale(1.005f, 1.005f);
	}
	if (RotateOn == true)
	{
		Act->Rotate(2.0f);
	}
}

void Ermine::App::OnDetach()
{
	Obj.OnDetach();
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


