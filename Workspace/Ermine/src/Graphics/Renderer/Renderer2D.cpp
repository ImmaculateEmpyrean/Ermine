#include "stdafx.h"
#include "Renderer2D.h"

//This Is an OpenGL Specific Renderer So Not a Big Deal..
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "OpenGLErrorChecker.h"

#include "2DPrimitives/Constructs/Quad.h"

#include "RendererPrimitives/VertexArray.h"
#include "RendererPrimitives/VertexBuffer.h"
#include "RendererPrimitives/IndexBuffer.h"

#include "Graphics/Renderer/MaterialSystem/Shader.h"
#include "Graphics/Renderer/MaterialSystem/Texture.h"

#include "2DPrimitives/Constructs/MovableObject.h"
#include "Graphics/Renderer/RenderableComponents/Renderable2D.h"
#include "Graphics/Renderer/RenderableComponents/RenderableTextureModule.h"
#include "Graphics/Renderer/RenderableComponents/RenderableShapeComponent.h"

#include "Physics/Physics.h"
#include "Physics/PhysicsComponent2D.h"
#include "RenderableComponents/RenderablePhysicsComponent2D.h"

#pragma region StaticDeclarationRegion

std::once_flag Ermine::Renderer2D::InitializationFlag;
Ermine::Renderer2D* Ermine::Renderer2D::GlobalRenderer2DObj;

#pragma endregion StaticDeclarationRegion

namespace Ermine
{
	Renderer2D::Renderer2D()
		:
		RendererLayerStack(std::string("Renderer2DLayerStack"))
	{}

	Renderer2D::~Renderer2D()
	{}


	Renderer2D* Renderer2D::Get()
	{
		std::call_once(InitializationFlag, []() {
			GlobalRenderer2DObj = new Renderer2D();
		});

		return GlobalRenderer2DObj;
	}


	void Renderer2D::BeginScene()
	{
		auto Renderer = Ermine::Renderer2D::Get();
		auto Lock = Renderer->GetObjectMutex();

		Renderer->SceneBegin = true;
	}

	void Renderer2D::Draw()
	{
		auto Renderer = Renderer2D::Get();
		auto Lock = Renderer->GetObjectMutex();

		Renderer2D::BeginScene();
		Renderer2D::EndScene();
	}

	void Renderer2D::SubmitLayer(LayerStackLayer layer)
	{
		auto Renderer = Ermine::Renderer2D::Get();
		auto Lock = Renderer->GetObjectMutex();

		Renderer->RendererLayerStack.PushLayerOnTheBackOfTheStack(std::make_unique<LayerStackLayer>(std::move(layer)));
	}

	void Renderer2D::SubmitLayer(LayerStackLayer layer, int index)
	{
		auto Renderer = Ermine::Renderer2D::Get();
		auto Lock = Renderer->GetObjectMutex();

		Renderer->RendererLayerStack.PushLayerOntoStackAtPosition(std::make_unique<LayerStackLayer>(std::move(layer)),index);
	}

	void Renderer2D::ReplaceLayerStackWithStack(LayerStack layerstack)
	{
		auto Renderer = Renderer2D::Get();
		auto Lock = Renderer2D::Get();

		Renderer->RendererLayerStack.Clear();
		Renderer->RendererLayerStack = std::move(layerstack);
	}

	void Renderer2D::EndScene()
	{
		auto Renderer = Ermine::Renderer2D::Get();
		auto Lock = Renderer2D::Get();

		assert(Renderer->SceneBegin); //Note Scene Must Have Begun To End Otherwise It cannot Be Ended..
		Renderer->SceneBegin = false;

		Renderer->DrawingRoutine();

#if defined(ER_DEBUG_DEVELOP) || defined(ER_DEBUG_SHIP)  //This Feature Is Only Available In Debug Mode.. (Its Way Too Expensive And Not Needed In Other Modes..)
		if(Renderer->UsePhysicsDebugger == true)
			Renderer->PhysicsDebuggerDrawingRoutine(); //Also The Physics Debugger Must Be Turned On 
#endif
		//Renderer->RendererLayerStack.Clear(); //Do Garbade Collection Here Instead Of Outright Clear..
	}

	void Renderer2D::TurnOnPhysicsDebugger()
	{
		auto Renderer = Ermine::Renderer2D::Get();
		auto Lock = Renderer->GetObjectMutex();

		Renderer->UsePhysicsDebugger = true;
	}
	void Renderer2D::TurnOffPhysicsDebugger()
	{
		auto Renderer = Ermine::Renderer2D::Get();
		auto Lock = Renderer->GetObjectMutex();

		Renderer->UsePhysicsDebugger = false;
	}


	void Renderer2D::SubmitPhysicsComponent2D(PhysicsComponent2D* PhyComp)
	{
		auto Renderer = Ermine::Renderer2D::Get();
		auto Lock = Renderer->GetObjectMutex();

		Renderer->PhysicsComponentsBuffer.emplace_back(PhyComp);
	}

	void Ermine::Renderer2D::RemovePhysicsComponent2D(PhysicsComponent2D* PhyComp)
	{
		auto Renderer = Ermine::Renderer2D::Get();
		auto Lock = Renderer->GetObjectMutex();

		bool Flag = false;
		int i;
		for (i = 0; i < Renderer->PhysicsComponentsBuffer.size(); i++)
		{
			if ((b2Body*)PhyComp == ((b2Body*)Renderer->PhysicsComponentsBuffer[i]))
			{
				//The Flag Represents That There Is Something We Want To Erase..
				Flag = true;
				break;
			}
		}
		if (Flag == true)
		{
			//Erase This Physics Component Right..
			Renderer->PhysicsComponentsBuffer.erase(Renderer->PhysicsComponentsBuffer.begin() + i);
		}

	}
	void Renderer2D::ClearPhysicsTrackingBuffer()
	{
		auto Renderer = Ermine::Renderer2D::Get();
		auto Lock = Renderer->GetObjectMutex();

		Renderer->PhysicsComponentsBuffer.clear();
	}


	void Renderer2D::SetCircleColor(glm::vec4 CircleShapeColor, glm::vec4 RadialLineColor)
	{
		auto Renderer = Renderer2D::Get();
		auto Lock = Renderer->GetObjectMutex();
		
		Renderer->CircleColor = CircleShapeColor;
		Renderer->CircleRadialLineColor = RadialLineColor;
	}
	void Renderer2D::SetPolygonColor(glm::vec4 PolygonColor)
	{
		auto Renderer = Renderer2D::Get();
		auto Lock = Renderer->GetObjectMutex();

		Renderer->PolygonColor = PolygonColor;
	}
	void Renderer2D::SetLineColor(glm::vec4 LineColor)
	{
		auto Renderer = Renderer2D::Get();
		auto Lock = Renderer->GetObjectMutex();

		Renderer->LineColor = LineColor;
	}


	void Renderer2D::DrawingRoutine()
	{
		auto Lock = GetObjectMutex();

		//Getting The Texture Cache As It May Prove Helpful..
		auto TextureCacheGlobal = Ermine::GlobalTextureCache::Get();
		auto Renderer = Ermine::Renderer2D::Get();
		auto Camera = Ermine::OrthographicCamera::Get();

		for (auto& layer : RendererLayerStack.LayersBuffer)
		{
			for (std::unique_ptr<Ermine::Renderable2D>& i : layer->Renderables)
			{
				auto Lock = i->GetObjectMutex();
				
				if (i->GetObjectInitialized() == false)
					continue;

				i->BindRenderable();
				i->GetMaterial()->GetShader()->UniformMat4(std::string("ModelMatrix"), i->GetModelMatrix());

				//Start If A Renderable Texture module Is detected Inside the renderable do the following steps//
				if (dynamic_cast <Ermine::RenderableTextureModule*>(&(*i)))
				{
					Ermine::RenderableTextureModule* Ptr = (RenderableTextureModule*)&(*i);
					std::vector<int> TextureArray = Ptr->BindTexturesContained();
					TextureArray.resize(16, 0);

					i->GetMaterial()->GetShader()->UniformNi(std::string("Sampler2DArray"),TextureArray);
				}
				//Ended If A Renderable Texture module Is detected Inside the renderable do the following steps//
	
				//Start These Steps Are To Be Done on All Kinds Of Renderables..//
				i->GetMaterial()->GetShader()->UniformMat4(std::string("ProjectionViewMatrix"), Camera->GetProjectionViewMatrix());
				glDrawElements(i->GetDrawMode(), i->GetVertexArray()->GetIndexBufferLength(), GL_UNSIGNED_INT, 0);
				//Ended These Steps Are To Be Done on All Kinds Of Renderables..//
			}
		}
	}

	void Renderer2D::PhysicsDebuggerDrawingRoutine()
	{
		auto Camera = Ermine::OrthographicCamera::Get();
		auto Renderer = Ermine::Renderer2D::Get();

		auto ForeignLock = Camera->GetObjectMutex();
		auto Lock = Renderer->GetObjectMutex();

		//Start Function Defaults Declarations..//
		static Ermine::Shader BasicDebuggerShader(std::filesystem::path("Shader/Vertex/PhysicsDebuggerVertexShader.vert"),
												  std::filesystem::path("Shader/Fragment/PhysicsDebuggerFragmentShader.frag"));

		static Ermine::Shader CircleShader(std::filesystem::path("Shader/Vertex/PhysicsDebuggerDrawCircle.vert"), 
										   std::filesystem::path("Shader/Fragment/PhysicsDebuggerDrawCircle.frag"));
		
		static std::vector<VertexAttribPointerSpecification> TriangleAndLineVertexArraySpecification = {
						   {3,GL_FLOAT,false},
						   {1,GL_FLOAT,false},
						   {4,GL_FLOAT,false}
		};

		static std::vector<VertexAttribPointerSpecification> CircleVertexArraySpecification = {
						   {3,GL_FLOAT,false},
						   {1,GL_FLOAT,false},
						   {2,GL_FLOAT,false},
						   {2,GL_FLOAT,false},
						   {1,GL_FLOAT,false},
						   {4,GL_FLOAT,false}
		};
		//Ended Function Defaults Declarations..//

		//Main Loop To Draw All Physics Components
		for (auto Component : PhysicsComponentsBuffer)
		{
			//Get The Main Physics Body From The Components In Question..
			b2Body* body = Component->BodyManagedByTheComponent;

			//Start Getting All Fixtures Associated With The Body..
			for (b2Fixture* f = body->GetFixtureList(); f; f = f->GetNext())
			{
				///Start Calculate Required Debug Color..//
				float UseVertexColor = 0.0f;
				glm::vec4 VertexColor = glm::vec4(1.0f);
				/*FixtureUserDataStruct* Struct = (FixtureUserDataStruct*)f->GetUserData();

				if (Struct != nullptr)
				{
					UseVertexColor = (float)Struct->DebugTraceColor.UseColor;
					VertexColor = Struct->DebugTraceColor.Color;

					if (Struct->DebugTraceColor.UseColor == false)
					{
						//This Branch Is Only Triggered When Fixture Color To Be Used Is False..
						UseVertexColor =(float)Component->UseCustomColorsOnDebugTrace; //If This Is Also False Then You Are Leaving The Renderer To Draw The Color..
						VertexColor = Component->CustomDebugTraceColor;
					}
				}
				///Ended Calculate Required Debug Color..//*/


				//Get The Type Of The Shape So That We Can Start Getting The Vertices..
				b2Shape::Type shapetype = f->GetType();

				//Start Extract Vertices From Polygon//
				if (shapetype == b2Shape::e_polygon)
				{
					//Buffers To Hold Triangle Data..//
					std::vector<float> TrianglesVertexBuffer;
					std::vector<uint32_t> TrianglesIndexBuffer;
					uint32_t TriangleIndexCounter = 0;

					//Start Getting VErtices And Indices Of The Polygon..
					b2PolygonShape* PolygonShape = (b2PolygonShape*)f->GetShape();
					
					//Get This Variable So That We Can Know From Where To Where This Shape Occupies VErtices.. We Also Need  The First One Right..
					uint32_t TriangleIndexStart = TriangleIndexCounter;

					for (int i = 0; i < PolygonShape->m_count; i++)
					{
						b2Vec2 BodyPostionB2Vec2 = body->GetPosition();
						glm::vec2 BodyPosition = glm::vec2(BodyPostionB2Vec2.x, BodyPostionB2Vec2.y);

						glm::vec2 Vertex = glm::vec2(PolygonShape->m_vertices[i].x, PolygonShape->m_vertices[i].y); //This Is Returning Garbage Fix It..

						Vertex = Vertex; //+ BodyPosition;

						glm::vec2 VertexInPixelSpace = Ermine::vectorWorldToErmine(Vertex);
						glm::vec4 VertexInPixelSpace4 = glm::vec4(VertexInPixelSpace, 0.0f, 0.0f);

						VertexInPixelSpace4 = Component->GetRotationMatrix() * VertexInPixelSpace4;

						//Vertex Coordinates In Pixel Coordinates..
						TrianglesVertexBuffer.emplace_back(VertexInPixelSpace4.x);
						TrianglesVertexBuffer.emplace_back(VertexInPixelSpace4.y);
						TrianglesVertexBuffer.emplace_back(3.0f);

						//Should I Use Vertex Color 0 is false..
						TrianglesVertexBuffer.emplace_back(UseVertexColor);

						//Vertex Color..
						TrianglesVertexBuffer.emplace_back(VertexColor.x);
						TrianglesVertexBuffer.emplace_back(VertexColor.y);
						TrianglesVertexBuffer.emplace_back(VertexColor.z);
						TrianglesVertexBuffer.emplace_back(VertexColor.w);

						TriangleIndexCounter++;
					}

					int Sc = TriangleIndexStart + 1;
					int Tr = TriangleIndexStart + 2;
					for (int i = TriangleIndexStart; Tr < TriangleIndexCounter; i++)
					{
						TrianglesIndexBuffer.emplace_back(TriangleIndexStart);
						TrianglesIndexBuffer.emplace_back(Sc);
						TrianglesIndexBuffer.emplace_back(Tr);

						Sc++;
						Tr++;
					}
					VertexArray TriangleVertexArray(TrianglesVertexBuffer, TrianglesIndexBuffer);
					TriangleVertexArray.SetVertexAttribArray(TriangleAndLineVertexArraySpecification);
					TriangleVertexArray.Bind();

					BasicDebuggerShader.Bind();
					BasicDebuggerShader.UniformMat4(std::string("ProjectionViewMatrix"), Camera->GetProjectionViewMatrix());
					BasicDebuggerShader.UniformMat4(std::string("ModelMatrix"), Component->GetTranslationMatrix()); //This Is The Only Reason Why We Are Drawing Per Body.. 
					BasicDebuggerShader.Uniform4f(std::string("Body_Color_Uniform"), PolygonColor);
					glDrawElements(GL_TRIANGLES, TriangleVertexArray.GetIndexBufferLength(), GL_UNSIGNED_INT, 0);
				}
				//Ended Extract Vertices From Polygon//

				//Start Extract Vertices From Edge Shape//
				if(shapetype == b2Shape::e_edge)
				{
					//Buffers To Hold Line Data..//
					std::vector<float> LineVertexBuffer;
					std::vector<uint32_t> LineIndexBuffer;
					uint32_t LineIndexCounter = 0;

					b2EdgeShape* EdgeShape = (b2EdgeShape*)f->GetShape();

					b2Vec2 P1 = EdgeShape->m_vertex1;
					b2Vec2 P2 = EdgeShape->m_vertex2;

					glm::vec2 PixelPoint1 = Ermine::vertexWorldToErmine(P1.x, P1.y);
					glm::vec2 PixelPoint2 = Ermine::vertexWorldToErmine(P2.x, P2.y);

					//Vertex 0
					//Vertex Coordinates In Pixel Space
					LineVertexBuffer.emplace_back(PixelPoint1.x);
					LineVertexBuffer.emplace_back(PixelPoint1.y);
					LineVertexBuffer.emplace_back(1.0f);

					//Should I Use Vertex Color 0 is false..
					LineVertexBuffer.emplace_back(UseVertexColor);

					//Vertex Color..
					LineVertexBuffer.emplace_back(VertexColor.x);
					LineVertexBuffer.emplace_back(VertexColor.y);
					LineVertexBuffer.emplace_back(VertexColor.z);
					LineVertexBuffer.emplace_back(VertexColor.w);
					//Ended Vertex 0

					//Vertex 1
					//Vertex Coordinates In Pixel Space
					LineVertexBuffer.emplace_back(PixelPoint2.x);
					LineVertexBuffer.emplace_back(PixelPoint2.y);
					LineVertexBuffer.emplace_back(1.0f);

					//Should I Use Vertex Color 0 is false..
					LineVertexBuffer.emplace_back(UseVertexColor);

					//Vertex Color..
					LineVertexBuffer.emplace_back(VertexColor.x);
					LineVertexBuffer.emplace_back(VertexColor.y);
					LineVertexBuffer.emplace_back(VertexColor.z);
					LineVertexBuffer.emplace_back(VertexColor.w);
					//Ended Vertex 1

					LineIndexBuffer.emplace_back(LineIndexCounter++);
					LineIndexBuffer.emplace_back(LineIndexCounter++);

					VertexArray LineVertexArray(LineVertexBuffer, LineIndexBuffer);
					LineVertexArray.SetVertexAttribArray(TriangleAndLineVertexArraySpecification);
					LineVertexArray.Bind();

					BasicDebuggerShader.Bind();
					BasicDebuggerShader.UniformMat4(std::string("ProjectionViewMatrix"), Camera->GetProjectionViewMatrix());
					BasicDebuggerShader.UniformMat4(std::string("ModelMatrix"), Component->GetTranslationMatrix()); //This Is The Only Reason Why We Are Drawing Per Body.. 
					BasicDebuggerShader.Uniform4f(std::string("Body_Color_Uniform"), LineColor);
					glDrawElements(GL_LINE_STRIP, LineVertexArray.GetIndexBufferLength(), GL_UNSIGNED_INT, 0);
				}
				//Ended Extract Vertices From Edge Shape//

				//Start Extract Vertices From Chain Shape..//
				if (shapetype == b2Shape::e_chain)
				{
					//Buffers To Hold Line Data..//
					std::vector<float> LineVertexBuffer;
					std::vector<uint32_t> LineIndexBuffer;
					uint32_t LineIndexCounter = 0;

					b2ChainShape* ChainShape = (b2ChainShape*)f->GetShape();
					
					for (int i = 0; i < ChainShape->m_count; i++)
					{
						b2Vec2 Vertex = ChainShape->m_vertices[i];

						glm::vec2 VertexInPixelSpace = Ermine::vertexWorldToErmine(glm::vec2(Vertex.x, Vertex.y));
						//VertexInPixelSpace.y = -1.0f * VertexInPixelSpace.y;

						LineVertexBuffer.emplace_back(VertexInPixelSpace.x);
						LineVertexBuffer.emplace_back(VertexInPixelSpace.y);
						LineVertexBuffer.emplace_back(3.0f);

						//Should I Use Vertex Color 0 is false..
						LineVertexBuffer.emplace_back(UseVertexColor);

						//Vertex Color..
						LineVertexBuffer.emplace_back(VertexColor.x);
						LineVertexBuffer.emplace_back(VertexColor.y);
						LineVertexBuffer.emplace_back(VertexColor.z);
						LineVertexBuffer.emplace_back(VertexColor.w);

						LineIndexBuffer.emplace_back(LineIndexCounter++);
					}

					VertexArray LineVertexArray(LineVertexBuffer, LineIndexBuffer);
					LineVertexArray.SetVertexAttribArray(TriangleAndLineVertexArraySpecification);
					LineVertexArray.Bind();

					BasicDebuggerShader.Bind();
					BasicDebuggerShader.UniformMat4(std::string("ProjectionViewMatrix"), Camera->GetProjectionViewMatrix());
					BasicDebuggerShader.UniformMat4(std::string("ModelMatrix"), Component->GetTranslationMatrix()); //This Is The Only Reason Why We Are Drawing Per Body.. 
					BasicDebuggerShader.Uniform4f(std::string("Body_Color_Uniform"), LineColor);
					glDrawElements(GL_LINE_STRIP, LineVertexArray.GetIndexBufferLength(), GL_UNSIGNED_INT, 0);
				}
				//Ended Extract Vertices From Chain Shape..//

				//Start Extract Vertices To Draw A Circle..//
				if (shapetype == b2Shape::e_circle)
				{
					//Start Circular Buffers..//

					//Buffers To Hold Circle Data..//
					std::vector<float> CircleVertexBuffer;
					std::vector<uint32_t> CircleIndexBuffer;
					uint32_t CircleIndexCounter = 0;

					//Buffers To Hold Circular Line Data..//
					std::vector<float> CircleLineVertexBuffer;
					std::vector<uint32_t> CircleLineIndexBuffer;
					uint32_t CircleLineIndexCounter = 0;

					//Ended Circular Buffers

					b2CircleShape* CircleShape = (b2CircleShape*)f->GetShape();

					float radius = Ermine::scalarWorldToErmine(CircleShape->m_radius);
					//float radius = CircleShape->m_radius;

					b2Vec2 OffsetFromCentre = CircleShape->m_p;
					b2Vec2 BodyPos = body->GetPosition();

					BodyPos = BodyPos + OffsetFromCentre;

					glm::vec2 PositionInPixelSpace= Ermine::coordWorldToErmine(glm::vec2(BodyPos.x, BodyPos.y));
					
					//Vertex 0
					CircleVertexBuffer.emplace_back(radius);
					CircleVertexBuffer.emplace_back(-1.0f * radius);
					CircleVertexBuffer.emplace_back(1.0f);
					CircleVertexBuffer.emplace_back(radius);
					CircleVertexBuffer.emplace_back((float)Ermine::GetScreenWidth());
					CircleVertexBuffer.emplace_back((float)Ermine::GetScreenHeight());
					CircleVertexBuffer.emplace_back(PositionInPixelSpace.x);
					CircleVertexBuffer.emplace_back(PositionInPixelSpace.y);

					//Flag If I Should Be Using Vertex Color
					CircleVertexBuffer.emplace_back(UseVertexColor);

					//Vertex Color To Be Packaged Into The VertexBuffer
					CircleVertexBuffer.emplace_back(VertexColor.x);
					CircleVertexBuffer.emplace_back(VertexColor.y);
					CircleVertexBuffer.emplace_back(VertexColor.z);
					CircleVertexBuffer.emplace_back(VertexColor.w);

					//Vertex 1
					CircleVertexBuffer.emplace_back(radius);
					CircleVertexBuffer.emplace_back(radius);
					CircleVertexBuffer.emplace_back(1.0f);
					CircleVertexBuffer.emplace_back(radius);
					CircleVertexBuffer.emplace_back((float)Ermine::GetScreenWidth());
					CircleVertexBuffer.emplace_back((float)Ermine::GetScreenHeight());
					CircleVertexBuffer.emplace_back(PositionInPixelSpace.x);
					CircleVertexBuffer.emplace_back(PositionInPixelSpace.y);

					CircleVertexBuffer.emplace_back(UseVertexColor);

					//Vertex Color To Be Packaged Into The VertexBuffer
					CircleVertexBuffer.emplace_back(VertexColor.x);
					CircleVertexBuffer.emplace_back(VertexColor.y);
					CircleVertexBuffer.emplace_back(VertexColor.z);
					CircleVertexBuffer.emplace_back(VertexColor.w);

					//Vertex 2
					CircleVertexBuffer.emplace_back(-1.0f * radius);
					CircleVertexBuffer.emplace_back(radius);
					CircleVertexBuffer.emplace_back(1.0f);
					CircleVertexBuffer.emplace_back(radius);
					CircleVertexBuffer.emplace_back((float)Ermine::GetScreenWidth());
					CircleVertexBuffer.emplace_back((float)Ermine::GetScreenHeight());
					CircleVertexBuffer.emplace_back(PositionInPixelSpace.x);
					CircleVertexBuffer.emplace_back(PositionInPixelSpace.y);

					CircleVertexBuffer.emplace_back(UseVertexColor);

					//Vertex Color To Be Packaged Into The VertexBuffer
					CircleVertexBuffer.emplace_back(VertexColor.x);
					CircleVertexBuffer.emplace_back(VertexColor.y);
					CircleVertexBuffer.emplace_back(VertexColor.z);
					CircleVertexBuffer.emplace_back(VertexColor.w);

					//Vertex 3
					CircleVertexBuffer.emplace_back(-1.0f * radius);
					CircleVertexBuffer.emplace_back(-1.0f * radius);
					CircleVertexBuffer.emplace_back(1.0f);
					CircleVertexBuffer.emplace_back(radius);
					CircleVertexBuffer.emplace_back((float)Ermine::GetScreenWidth());
					CircleVertexBuffer.emplace_back((float)Ermine::GetScreenHeight());
					CircleVertexBuffer.emplace_back(PositionInPixelSpace.x);
					CircleVertexBuffer.emplace_back(PositionInPixelSpace.y);

					CircleVertexBuffer.emplace_back(UseVertexColor);

					//Vertex Color To Be Packaged Into The VertexBuffer
					CircleVertexBuffer.emplace_back(VertexColor.x);
					CircleVertexBuffer.emplace_back(VertexColor.y);
					CircleVertexBuffer.emplace_back(VertexColor.z);
					CircleVertexBuffer.emplace_back(VertexColor.w);


					std::vector<uint32_t> IndexBuffer = Quad::GetModelIndices();
					
					for (auto& i : IndexBuffer)
						CircleIndexBuffer.emplace_back(i + CircleIndexCounter);
					
					CircleIndexCounter = CircleIndexCounter + 4;

					//Start Writing Into Circle Line Buffer

					glm::mat4 RotationMatrix(1.0f);
					RotationMatrix = glm::rotate<float>(RotationMatrix, Component->GetAngleOfTheBodyRadians(), glm::vec3(0.0, 0.0, 1.0));
					
					//Vertex 0
					glm::vec4 Vertex0(1.0);
					Vertex0.x = 0.0f;
					Vertex0.y = -1.0 * CircleShape->m_radius;
					Vertex0.z = 1.0f;
					
					Vertex0 = RotationMatrix * Vertex0;

					glm::vec2 Vertex0PixelSpace = Ermine::vertexWorldToErmine(glm::vec2(Vertex0.x , Vertex0.y));

					CircleLineVertexBuffer.emplace_back(Vertex0PixelSpace.x);
					CircleLineVertexBuffer.emplace_back(Vertex0PixelSpace.y);
					CircleLineVertexBuffer.emplace_back(1.0f);

					//Should I Use Vertex Color 0 is false..
					CircleLineVertexBuffer.emplace_back(0.0f);

					//Vertex Color..
					CircleLineVertexBuffer.emplace_back(1.0f);
					CircleLineVertexBuffer.emplace_back(0.0f);
					CircleLineVertexBuffer.emplace_back(0.0f);
					CircleLineVertexBuffer.emplace_back(0.4f);

					//Vertex 1
					glm::vec4 Vertex1(1.0f);
					Vertex1.x = 0.0f;
					Vertex1.y = 1.0 * CircleShape->m_radius;
					Vertex1.z = 1.0f;

					Vertex1 = RotationMatrix * Vertex1;

					glm::vec2 Vertex1PixelSpace = Ermine::vertexWorldToErmine(glm::vec2(Vertex1.x, Vertex1.y));

					CircleLineVertexBuffer.emplace_back(Vertex1PixelSpace.x);
					CircleLineVertexBuffer.emplace_back(Vertex1PixelSpace.y);
					CircleLineVertexBuffer.emplace_back(1.0f);

					//Should I Use Vertex Color 0 is false..
					CircleLineVertexBuffer.emplace_back(0.0f);

					//Vertex Color..
					CircleLineVertexBuffer.emplace_back(1.0f);
					CircleLineVertexBuffer.emplace_back(0.0f);
					CircleLineVertexBuffer.emplace_back(0.0f);
					CircleLineVertexBuffer.emplace_back(0.4f);

					//Ended Writing Into Circle Line Buffer
					CircleLineIndexBuffer.emplace_back(CircleLineIndexCounter++);
					CircleLineIndexBuffer.emplace_back(CircleLineIndexCounter++);

					//Start Draw Circle..//

					VertexArray CircleVertexArray(CircleVertexBuffer, CircleIndexBuffer);
					CircleVertexArray.SetVertexAttribArray(CircleVertexArraySpecification);
					CircleVertexArray.Bind();

					CircleShader.Bind();
					CircleShader.UniformMat4(std::string("ProjectionViewMatrix"), Camera->GetProjectionViewMatrix());
					CircleShader.UniformMat4(std::string("ModelMatrix"), Component->GetTranslationMatrix()); //This Is The Only Reason Why We Are Drawing Per Body.. 
					CircleShader.Uniform4f(std::string("Uniform_CircleColor"), CircleColor);
					glDrawElements(GL_TRIANGLES, CircleVertexArray.GetIndexBufferLength(), GL_UNSIGNED_INT, 0);

					VertexArray CircleLineVertexArray(CircleLineVertexBuffer, CircleLineIndexBuffer); //For Some Reason There Is A Problem If We Try To Multiply The Model Matrix.. :< Dunno Check It Out Tommorow
					CircleLineVertexArray.SetVertexAttribArray(TriangleAndLineVertexArraySpecification);
					CircleLineVertexArray.Bind();

					BasicDebuggerShader.Bind();
					BasicDebuggerShader.UniformMat4(std::string("ProjectionViewMatrix"), Camera->GetProjectionViewMatrix());
					BasicDebuggerShader.UniformMat4(std::string("ModelMatrix"), Component->GetTranslationMatrix()); //This Is The Only Reason Why We Are Drawing Per Body.. 
					BasicDebuggerShader.Uniform4f(std::string("Body_Color_Uniform"), CircleRadialLineColor);
					glDrawElements(GL_LINE_STRIP, CircleLineVertexArray.GetIndexBufferLength(), GL_UNSIGNED_INT, 0);
					//Ended Draw Circle..//
				}
				//Ended Extract Vertices To Draw A Circle..//
			}
		}
	}
}
