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
	//static Ermine::PhysicsComponent2D TopBeam = Ermine::PhysicsComponent2D();
	static Ermine::PhysicsComponent2D TopBeam(glm::vec2(500.0f, 25.0f),glm::vec2(1000.0f,25.0f),true,glm::vec4(1.0f,0.0f,0.0f,1.0f));
	static Ermine::PhysicsComponent2D BottomBeam(glm::vec2(500.0f, 988.0f), glm::vec2(1000.0f, 25.0f), true, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	
	static Ermine::PhysicsComponent2D LeftBeam(glm::vec2(12.0f, 500.0f), glm::vec2(25.0f, 1000.0f), true, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	static Ermine::PhysicsComponent2D RightBeam(glm::vec2(988.0f, 500.0f), glm::vec2(25.0f, 1000.0f), true, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

	static Ermine::PhysicsComponent2D CarBody(glm::vec2(500.0f,500.0f), glm::vec2(100.0f, 50.0f),false);

	static Ermine::PhysicsComponent2D RightAxle(glm::vec2(530.0f, 545.0f), glm::vec2(10.0f, 10.0f), false);
	static Ermine::PhysicsComponent2D LeftAxle(glm::vec2(465.0f, 545.0f), glm::vec2(10.0f, 10.0f), false);

	static Ermine::PhysicsComponent2D RightWheel(glm::vec2(530.0f, 545.0f), 14.0f, false,glm::vec4(1.0f,1.0f,1.0f,0.4f));
	static Ermine::PhysicsComponent2D LeftWheel(glm::vec2(465.0f, 545.0f), 14.0f, false);

	static bool Once = true;

	if (Once)
	{
		Once = false;
		Renderer2D::TurnOnPhysicsDebugger();

		TopBeam.StartDebugTrace();
		BottomBeam.StartDebugTrace();

		LeftBeam.StartDebugTrace();
		RightBeam.StartDebugTrace();

		//CarBody.StartDebugTrace();

		//RightAxle.StartDebugTrace();
		//LeftAxle.StartDebugTrace();

		//RightWheel.StartDebugTrace();
		//LeftWheel.StartDebugTrace();

		CarBody.SetDebugColorToBody(glm::vec4(0.0f, 0.0f, 1.0f, 0.4f));
		RightAxle.CreateRevoluteJoint(&RightWheel, glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), false);
		LeftAxle.CreateRevoluteJoint(&LeftWheel, glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), false);

		//CarBody.CreateRevoluteJoint(&RightWheel, glm::vec2(50.0f, 25.0f), glm::vec2(0.0f, 0.0f), false);

		CarBody.CreateWheelJoint(&RightAxle);
		//CarBody.CreateWheelJoint(&RightWheel);
		CarBody.CreateWheelJoint(&LeftAxle);
		//CarBody.CreateWheelJoint(&LeftWheel);

		
		//auto Joint  = CarBody.CreatePrismaticJoint(&RightAxle, glm::vec2(30.0f, 50.0f), glm::vec2(0.0f, 0.0f),0.2f, false);
		//auto Joint2 = CarBody.CreatePrismaticJoint(&LeftAxle, glm::vec2(-30.0f, 50.0f), glm::vec2(0.0f, 0.0f),0.2f, false);

		//Ermine::PrismaticJoint* PrismaticJoint = (Ermine::PrismaticJoint*)Joint;
		//PrismaticJoint->SetMovementLimits(0.0f, 0.0f);
		
		//Ermine::PrismaticJoint* PrismaticJoint2 = (Ermine::PrismaticJoint*)Joint2;
		//PrismaticJoint2->SetMovementLimits(0.0f, 0.0f);
	}

	//Start Add A Image To The Physics Component//

	static auto TextureCache =  Ermine::GlobalTextureCache::Get();
	static auto Tex = TextureCache->GetTextureFromFile("Texture/mini-topless.png");
	static Ermine::Sprite* Spr = new Ermine::Sprite(Tex, { 0.0f,0.0f }, { 1.0f,1.0f });

	static auto WheelTex = TextureCache->GetTextureFromFile("Texture/wheel-front.png");
	static Ermine::Sprite* WheelSpr = new Ermine::Sprite(WheelTex, { 0.0f,0.0f }, { 1.0f,1.0f });

	static bool OnceFlag = false;

	static std::shared_ptr<Ermine::Sprite> SprPointer;

	static std::shared_ptr<Ermine::Sprite> WheelPointer;
	if (OnceFlag == false)
	{
		OnceFlag = true;

		SprPointer.reset(Spr);

		WheelPointer.reset(WheelSpr);
	}
	static Ermine::PhysicsActor* PhyActor = new Ermine::PhysicsActor(SprPointer, std::move(CarBody));

	static Ermine::PhysicsActor* WheelActorLeft = new Ermine::PhysicsActor(WheelPointer, std::move(LeftWheel));
	static Ermine::PhysicsActor* WheelActorRight = new Ermine::PhysicsActor(WheelPointer, std::move(RightWheel));

	LayerStackLayer Layer("Han");
	Layer.SubmitRenderable(PhyActor);
	Layer.SubmitRenderable(WheelActorLeft);
	Layer.SubmitRenderable(WheelActorRight);

	//Ended Add A Image To The Physics Component//

	Renderer2D::BeginScene();
	Renderer2D::SubmitLayer(Layer);
	Renderer2D::EndScene();

	static float ForceAppliedBox1[2];
	static float ForceAppliedBox2[2];

	bool ApplyForce = false;
	bool RandomForceApplication = false;

	ImGui::Begin("Physics Test ");

	ImGui::InputFloat2("##PhysicsTestOnBox1", ForceAppliedBox1, 2);
	ImGui::SameLine();
	if (ImGui::Button("Reverse Force Applied"))
	{
		ForceAppliedBox1[0] = ForceAppliedBox1[0] * -1;
		ForceAppliedBox1[1] = ForceAppliedBox1[1] * -1;
	}
	/*-------------------------------------------------------------*/
	ImGui::InputFloat2("##PhysicsTestOnBox2", ForceAppliedBox2, 2);
	ImGui::SameLine();
	if (ImGui::Button("Reverse Force Applied"))
	{
		ForceAppliedBox2[0] = ForceAppliedBox2[0] * -1;
		ForceAppliedBox2[1] = ForceAppliedBox2[1] * -1;
	}

	/*-------------------------------------------------------------*/
	ImGui::Text("Apply Force Randomly!!");
	ImGui::SameLine();
	ImGui::Checkbox("##AplyForceRandomlyCheckBox", &RandomForceApplication);

	if (RandomForceApplication == false)
	{
		if (ImGui::Button("ApplyForce!!!##PhysicsTestApplyForceOnBox"))
			ApplyForce = true;
	}
	ImGui::End();

	if (ApplyForce)
	{
		PhyActor->AddForceToCentre(glm::vec2(ForceAppliedBox1[0], ForceAppliedBox1[1]));
		auto Camera = Ermine::OrthographicCamera::Get();

		Camera->TranslateCamera({ 10.0f,0.0f });
		//WheelActorRight->AddForceToCentre(glm::vec2(ForceAppliedBox1[0], ForceAppliedBox1[1]));
		//RightAxle.AddForceToCentre(glm::vec2(ForceAppliedBox1[0], ForceAppliedBox1[1]));
		//RightWheel.AddForceToCentre(glm::vec2(ForceAppliedBox1[0], ForceAppliedBox1[1]));
	}
	if (RandomForceApplication)
	{
		
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


