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
	WindowHandler::GlobalWindowHandler->SubmitWindowFront(std::make_unique<DebugMainWindow>());
	//Ended Create Window Handler..//
	
	//Start Setup Physics Of the engine..//
	
	//Create A Box2d Universe
	Universum = new b2World(b2Vec2(PhysicsConfig.Gravity.x, PhysicsConfig.Gravity.y));
	
	//Compute Box2d Physics World Bounds..
	Ermine::RecalculatePhysicsWorldBounds();

	//Ended Setup Physics of The Engine..//

	//Start Test Ground//
	glm::vec2 LocPixelCoordinates = { 500.0f,500.0f };//{ Ermine::GetScreenWidth()/2,Ermine::GetScreenHeight()/2};
	glm::vec2 Loc = Ermine::coordPixelsToWorld(LocPixelCoordinates);

	b2BodyDef GroundBodyDefinition;
	GroundBodyDefinition.position.Set(0.0f,-5.0f);
	GroundBodyDefinition.type = b2_staticBody;

	static auto GroundShape = b2PolygonShape();
	GroundShape.SetAsBox(5.0f, 0.001f);

	b2FixtureDef GroundFixture;
	GroundFixture.density = 1.0f;
	GroundFixture.shape = &GroundShape;
	GroundFixture.friction = 0.3f;

	b2Body* GroundBody;
	GroundBody = Universum->CreateBody(&GroundBodyDefinition);

	GroundBody->CreateFixture(&GroundFixture);

	//Ended Test Ground//

	//Start Box2D Checker//
	/*Universum = new b2World(b2Vec2(PhysicsConfig.Gravity.x, PhysicsConfig.Gravity.y));

	b2BodyDef GroundBodyDefinition;
	GroundBodyDefinition.position.Set(0.0f, -20.0f);

	b2Body* GroundBody = Universum->CreateBody(&GroundBodyDefinition);

	b2PolygonShape GroundShape;
	GroundShape.SetAsBox(50.0f, 10.0f);

	GroundBody->CreateFixture(&GroundShape, 0.0f);

	b2BodyDef CubeBodyDefinition;
	CubeBodyDefinition.position.Set(0.0f, 10.0f);
	CubeBodyDefinition.type = b2_dynamicBody;

	b2Body* CubeBody = Universum->CreateBody(&CubeBodyDefinition);

	b2PolygonShape BoxShape;
	BoxShape.SetAsBox(10.f, 5.0f);

	b2FixtureDef CubeBodyFixtureDefinition;
	CubeBodyFixtureDefinition.shape = &BoxShape;
	CubeBodyFixtureDefinition.density = 1.0f;
	CubeBodyFixtureDefinition.friction = 0.3f;

	CubeBody->CreateFixture(&CubeBodyFixtureDefinition);

	float timestep = 1.0f / 60.0f;
	int32 velocityiterations = 6;
	int32 positionIterations = 2;

	b2Vec2 Position = CubeBody->GetPosition();
	float angle = CubeBody->GetAngle();
	std::cout << "Position : [" << Position.x << "," << Position.y << "]" << " Angle : " << angle << std::endl;

	for (int i = 0; Position.y>=1.0f;i++)
	{
		Universum->Step(timestep, velocityiterations, positionIterations);

		Position = CubeBody->GetPosition();
		angle = CubeBody->GetAngle();

		std::cout << "Position : [" << Position.x << "," << Position.y << "]" << " Angle : " << angle << std::endl;
	}

	//Ended Box2D Checker//*/

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
	
	//Start Test Block Delete After Usuage
	glfwSwapInterval(0); //Disable VSYNC
	//Ended Test Block Delete After Usuage

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

	/*static bool Coke = false;
	
	auto FontCache = GlobalFontCache::Get();
	FontCache->SetFontSize(80);
	FontCache->LoadFontFromFile(std::filesystem::path("CalibriLight.ttf"));

	auto fonttest = (*FontCache)["CalibriLight"];

	glm::vec3 Color = { 1.0f,1.0f,1.0f };

	static Ermine::Label Lab("Hello World", Color,fonttest);

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
		//Lab.Scale({ 1.02f,1.02f });
	}*/


	/*//STDOUTLog_Trace("TimeStep : {0}", Ermine::TimeStep.GetSeconds());

	//Start SpriteBook Test//
	//The Example Is So Horrible Because Of The Way The Tileset Loads The Sprites.. It loads from top to bottom first.. however more testing is absolutely required to determine if the spritebook is even working.. 
	static Ermine::TileSet Scarlet(std::filesystem::path("TileSet/Scarlet.json"));
	static Ermine::Actor2D* Act = new Ermine::Actor2D(Scarlet.GetSpriteBuffer());
	
	glm::mat4 Camera = glm::mat4(1.0f);
	glm::translate(Camera, glm::vec3(0.0f, 0.0f, -3.0f));

	LayerStackLayer Layer("Han");
	Layer.SubmitRenderable(Act);
	auto ProjectionMatrix = glm::ortho<float>(0.0f, ((float)Ermine::GetScreenWidth()), ((float)Ermine::GetScreenHeight()), 0.0f, -5.0f, 5.0f);

	Renderer2D::BeginScene(Camera, ProjectionMatrix);

	Renderer2D::SubmitLayer(Layer);

	Renderer2D::EndScene();


	static glm::vec2 Translate = glm::vec2(500.0f,600.0f);
	static float Rotate = 4.7f;
	static glm::vec2 Scale = glm::vec2(1.0f, 1.0f);

	ImGui::Begin("Control Panel");

	ImGui::InputFloat("Translate X :", &Translate.x, 1.0f, 2.0f, 4);
	ImGui::InputFloat("Translate Y :", &Translate.y, 1.0f,2.0f, 4);
	
	ImGui::Separator();

	ImGui::InputFloat("Rotate Angle :", &Rotate, 0.1f, 0.2f, 4);
	
	ImGui::Separator();

	ImGui::InputFloat("Scale X : ", &Scale.x, 0.1f, 0.2f, 4);
	ImGui::InputFloat("Scale Y : ", &Scale.y, 0.1f, 0.2f, 4);

	ImGui::End();


	Act->ClearRotations();
	Act->Rotate(Rotate, true);

	Act->ClearTranslations();
	Act->Translate(Translate);

	Act->ClearScale();
	Act->Scale(Scale);

	//Ended SpriteBook Test//*/

#pragma region PhysicsComponentTest

	//Start Physics Component Test//
	
	glm::vec2 LocPixelCoordinates = { 500.0f,0.0f };//{ Ermine::GetScreenWidth()/2,Ermine::GetScreenHeight()/2};
	glm::vec2 Loc = Ermine::coordPixelsToWorld(LocPixelCoordinates);

	b2BodyDef Def;
	Def.position.Set(Loc.x,Loc.y);
	Def.type = b2_dynamicBody;

	static auto BoxShape = b2PolygonShape();
	BoxShape.SetAsBox(10.0f, 10.0f);

	b2FixtureDef FDef;
	FDef.density = 1.0f;
	FDef.shape = &BoxShape;
	FDef.friction = 0.3f;

	static PhysicsComponent2D Obj(Def, FDef,glm::vec2(10.0f,10.0f));

	//static PhysicsComponent2D GroundBody(GroundBodyDefinition,GroundFixture,glm::vec2(50.0f,2.0f));

	//b2Vec2 Position = Obj.operator b2Body* ()->GetPosition();
	//float angle = Obj.operator b2Body * ()->GetAngle();

	//std::cout << "Position : [" << Position.x << "," << Position.y << "]" << " Angle : " << angle << std::endl;
	//std::cout << Obj.operator b2Body * ()->GetPosition().y<<std::endl;

	//Ended Physics Component Test//

#pragma endregion PhysicsComponentTest

#pragma region PhysicsActorTest
	static bool l = true;
	//Ermine::Material mat(std::filesystem::path("Shader/Actor2DBaseMaterial.json"));

	auto Manager = Ermine::GlobalTextureCache::Get();
	static auto Tex = Manager->GetTextureFromFile("Texture/BoxSprite.png");
	static auto GroundTex = Manager->GetTextureFromFile("Texture/FloorTile.png");

	static Sprite* spr = new Sprite(Tex, { 0.0f,0.0f }, { 1.0f,1.0f });
	static Sprite* Grndspr = new Sprite(GroundTex, { 0.0f,0.0f }, { 1.0f,1.0f });

	static std::shared_ptr<Sprite> ShSpr;
	static std::shared_ptr<Sprite> GrndShSpr;

	if (l == true)
	{
		ShSpr.reset(spr);
		GrndShSpr.reset(Grndspr);
		l = false;
	}

	static Ermine::PhysicsActor* Act = new Ermine::PhysicsActor(ShSpr, std::move(Obj));
	//static Ermine::PhysicsActor* GrndAct = new Ermine::PhysicsActor(GrndShSpr, std::move(GroundBody));
	//static Ermine::Actor2D* Act = new Ermine::Actor2D(ShSpr);
	//Act->Translate({ 0.5f,0.5f });

	glm::mat4 Camera = glm::mat4(1.0f);
	glm::translate(Camera, glm::vec3(0.0f, 0.0f, -3.0f));

	LayerStackLayer Layer("Han");
	Layer.SubmitRenderable(Act);
	//Layer.SubmitRenderable(GrndAct);

	auto ProjectionMatrix = glm::ortho<float>(0.0f, ((float)Ermine::GetScreenWidth()), ((float)Ermine::GetScreenHeight()), 0.0f, -5.0f, 5.0f);//glm::ortho<float>(-1.0f, 1.0f, -1.0f, 1.0f, -5.0f, 5.0f);//glm::ortho<float>(-2.0f, 2.0f, -2.0f, 2.0f, -5.0f, 5.0f);//glm::ortho<float>(0.0f, ((float)Ermine::GetScreenWidth()), ((float)Ermine::GetScreenHeight()), 0.0f, -5.0f, 5.0f);//glm::ortho<float>(-2.0f, 2.0f, -2.0f, 2.0f, -5.0f, 5.0f);

	Renderer2D::BeginScene(Camera, ProjectionMatrix);
	Renderer2D::SubmitLayer(Layer);
	Renderer2D::EndScene();

#pragma endregion PhysicsActorTest
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
		PhysicsWorldInitializationStruct Phy;
		Phy.Gravity = glm::vec2(0.0f, -1.0f);		
		PointerToApp = new App("Ermine Development Environment", GetGameWindowDiamensions(),Phy);
#endif

	});

	return PointerToApp;
}


