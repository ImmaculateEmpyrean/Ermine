#include "stdafx.h"
#include "App.h"

#include "Graphics/Window/Window.h"

#include "glm.hpp"

#include "EventSystem/Components/BroadcastComponent.h"

Ermine::App::App(std::string AppTitle, std::pair<int, int> Diamensions)
	:
	AppTitle(AppTitle),
	Diamensions(Diamensions),
	AppLayerStack("AppMainLayerStack")
{
#if defined(ER_DEBUG_DEVELOP) || defined(ER_DEBUG_SHIP)
	//Start Initializing Log Library                  Donot Initialize Logs On Release Builds..
	std::vector<std::pair<std::string, CreateLogFile>> Configuration;
	Configuration.emplace_back(std::make_pair<std::string, CreateLogFile>("Augustus", CreateLogFile::CreateLogFile));
	Log::Init(Configuration);
	//Ended Initializing Log Library
#endif

	//Create A Window For The App..
	ManagedWindow = new Window(AppTitle, Diamensions);

	//Start Setup Physics Of the engine..
	Universum = new b2World(b2Vec2({ 0.0f,10.0f }));//Universum = new b2World(b2Vec2({BeginLevel.GetGravityVector().x,BeginLevel.GetGravityVector().y}));

	//Start Off Window Handler.. //TODO - Get Rid Of This Or Expand Its Api..
	//WindowHandler::GlobalWindowHandler = new WindowHandler();

	//Initialize Event Station
	auto Station = Ermine::EventBroadcastStation::GetStation();
}

Ermine::App::~App()
{
	//App Must Close All The Things it Handles..

	//Deallocate Box2D
	delete Universum;

	//Destroy Managed Window..
	delete ManagedWindow;

	//Finally Remove All Gui Being Drawn..
	//delete WindowHandler::GlobalWindowHandler;
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
	//Get The DeltaTime Calculation Which Is to Be Used By The Engine this Entire Frame..
	Ermine::TimeStep.Update();

	static double UpdateCounter = 0.0;
	UpdateCounter = UpdateCounter + TimeStep.GetSeconds();
	
	if (UpdateCounter >= 0.04f) //  1.0f/25.0f = 0.0f Update Cycle Is Locked To 25 fps..
	{
		UpdateLoop();
		Ermine::BroadcastComponent::BroadcastEvent(std::make_unique<Ermine::OnUpdateTickEvent>(UpdateCounter));
		UpdateCounter = 0.0;
		STDOUTDefaultLog_Trace("Update Tick Broadcasted");
	}
	else RenderLoop(UpdateCounter);
}

void Ermine::App::UpdateLoop()
{
	Universum->Step(PhysicsWorldTimestep , PhysicsVelocityIterations, PhysicsPositionIterations); //Since Update Cycle Is Locked To 25 fps I Have Decided To Hard Code 25 Value..
	Quit = ManagedWindow->ShouldIQuit();
}

void Ermine::App::RenderLoop(float DeltaTime)
{
	ManagedWindow->PreNewFrameProcess();
	Ermine::BroadcastComponent::BlockingBroadcast(std::make_unique<Ermine::OnRenderTickEvent>(DeltaTime));
	STDOUTDefaultLog_Trace("Render Tick Broadcasted");
	ManagedWindow->PostNewFrameProcess();
}


void Ermine::App::AddLayer(std::unique_ptr<Ermine::LayerStackLayer> Layer, int index)
{
	AppLayerStack.PushLayer(std::move(Layer), index);
}

void Ermine::App::DeleteLayer(std::string LayerName)
{
	//Forward The Call
	AppLayerStack.DeleteLayer(LayerName);
}
void Ermine::App::DeleteLayer(int IndexNumber)
{
	//Forward The Call
	AppLayerStack.DeleteLayer(IndexNumber);
}
void Ermine::App::DeleteLayer()
{
	//Forward The Call
	AppLayerStack.DeleteLayer();
}
