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


	void Renderer2D::BeginScene(glm::mat4 Camera, glm::mat4 ProjectionMatrix)
	{
		auto Renderer = Ermine::Renderer2D::Get();
		
		Renderer->CameraMatrix = Camera;
		Renderer->ProjectionMatrix = ProjectionMatrix;

		Renderer->ProjectionViewMatrix = ProjectionMatrix * Camera;

		Renderer->SceneBegin = true;
	}

	void Renderer2D::SubmitLayer(LayerStackLayer layer)
	{
		auto Renderer = Ermine::Renderer2D::Get();
		Renderer->RendererLayerStack.PushLayerOnTheBackOfTheStack(std::make_unique<LayerStackLayer>(layer));
	}

	void Renderer2D::SubmitLayer(LayerStackLayer layer, int index)
	{
		auto Renderer = Ermine::Renderer2D::Get();
		Renderer->RendererLayerStack.PushLayerOntoStackAtPosition(std::make_unique<LayerStackLayer>(layer),index);
	}

	void Renderer2D::ReplaceLayerStackWithStack(LayerStack layerstack)
	{
		auto Renderer = Renderer2D::Get();

		Renderer->RendererLayerStack.Clear();
		Renderer->RendererLayerStack = layerstack;
	}

	void Renderer2D::EndScene()
	{
		auto Renderer = Ermine::Renderer2D::Get();
		assert(Renderer->SceneBegin); //Note Scene Must Have Begun To End Otherwise It cannot Be Ended..
		Renderer->SceneBegin = false;

		Renderer->DrawingRoutine();

#ifdef ER_DEBUG_DEVELOP //This Feature Is Only Available In Debug Mode.. (Its Way Too Expensive And Not Needed In Other Modes..)
		if(Renderer->UsePhysicsDebugger == true)
			Renderer->PhysicsDebuggerDrawingRoutine(); //Also The Physics Debugger Must Be Turned On 
#endif
		Renderer->RendererLayerStack.Clear();
	}

	void Renderer2D::TurnOnPhysicsDebugger()
	{
		auto Renderer = Ermine::Renderer2D::Get();
		Renderer->UsePhysicsDebugger = true;
	}
	void Renderer2D::TurnOffPhysicsDebugger()
	{
		auto Renderer = Ermine::Renderer2D::Get();
		Renderer->UsePhysicsDebugger = false;
	}


	void Renderer2D::SubmitPhysicsComponent2D(PhysicsComponent2D* PhyComp)
	{
		auto Renderer = Ermine::Renderer2D::Get();
		Renderer->PhysicsComponentsBuffer.emplace_back(PhyComp);
	}
	void Renderer2D::ClearPhysicsTrackingBuffer()
	{
		auto Renderer = Ermine::Renderer2D::Get();
		Renderer->PhysicsComponentsBuffer.clear();
	}

	void Renderer2D::DrawingRoutine()
	{
		//Getting The Texture Cache As It May Prove Helpful..
		auto TextureCacheGlobal = Ermine::GlobalTextureCache::Get();
		auto Renderer = Ermine::Renderer2D::Get();

		for (auto layer : RendererLayerStack.AllLayersAssociated)
		{
			for (Renderable2D* i : layer->Renderables)
			{
				i->Bind();

				//Start If A Movable Object Is Detected Inside The Rendererable Do The Following Steps//
				if(dynamic_cast <Ermine::MovableObject*>(i))
				{
					i->Bind();
					
					MovableObject* MovObj = dynamic_cast<MovableObject*>(i);
					i->GetMaterialBeingUsed()->GetShader()->UniformMat4(std::string("ModelMatrix"), (MovObj)->GetModelMatrix());
				}
				//Ended If A Movable Object Is Detected Inside The Rendererable Do The Following Steps//

				//Start If A PhysicsComponent2D is detected inside the rendererable do the following steps//
				if (dynamic_cast <Ermine::RenderablePhysicsComponent2D*>(i))
				{
					i->Bind();

					RenderablePhysicsComponent2D* PhysicsObject = dynamic_cast<RenderablePhysicsComponent2D*>(i);
					i->GetMaterialBeingUsed()->GetShader()->UniformMat4(std::string("ModelMatrix"), (PhysicsObject)->GetTranslationMatrix());
				}
				//Ended If A PhysicsComponent2D is detected inside the rendererable do the following steps//

				//Start If A Renderable Texture module Is detected Inside the renderable do the following steps//
				if (dynamic_cast <Ermine::RenderableTextureModule*>(i))
				{
					i->Bind();

					Ermine::RenderableTextureModule* Ptr = (RenderableTextureModule*)i;
					std::vector<int> TextureArray = Ptr->BindTexturesContained();
					TextureArray.resize(16, 0);

					i->GetMaterialBeingUsed()->GetShader()->UniformNi(std::string("Sampler2DArray"),TextureArray);
				}
				//Ended If A Renderable Texture module Is detected Inside the renderable do the following steps//

				//Start These Steps Are To Be Done on All Kinds Of Renderables..//
				i->GetMaterialBeingUsed()->GetShader()->UniformMat4(std::string("ProjectionViewMatrix"), Renderer->ProjectionViewMatrix);
				glDrawElements(GL_TRIANGLES, i->GetVertexArray()->GetIndexBufferLength(), GL_UNSIGNED_INT, 0);
				//Ended These Steps Are To Be Done on All Kinds Of Renderables..//
			}
		}
	}

	void Renderer2D::PhysicsDebuggerDrawingRoutine()
	{
		
		auto Renderer = Ermine::Renderer2D::Get();

		//Start Function Defaults Declarations..//
		static Ermine::Shader BasicDebuggerShader(std::filesystem::path("Shader/Vertex/PhysicsDebuggerVertexShader.vert"),
												  std::filesystem::path("Shader/Fragment/PhysicsDebuggerFragmentShader.frag"));

		static Ermine::Shader CircleShader(std::filesystem::path("Shader/Vertex/PhysicsDebuggerDrawCircle.vert"), 
										   std::filesystem::path("Shader/Fragment/PhysicsDebuggerDrawCircle.frag"));
		
		static std::vector<VertexAttribPointerSpecification> TriangleAndLineVertexArraySpecification = {
						   {3,GL_FLOAT,false}
		};

		static std::vector<VertexAttribPointerSpecification> CircleVertexArraySpecification = {
						   {3,GL_FLOAT,false},
						   {1,GL_FLOAT,false},
						   {2,GL_FLOAT,false},
						   {2,GL_FLOAT,false}
		};
		//Ended Function Defaults Declarations..//

		//Main Loop To Draw All Physics Components
		for (auto Component : PhysicsComponentsBuffer)
		{
			//Start Declaration Of Buffers..//

			//Buffers To Hold Triangle Data..//
			std::vector<float> TrianglesVertexBuffer;
			std::vector<uint32_t> TrianglesIndexBuffer;
			uint32_t TriangleIndexCounter = 0;

			//Buffers To Hold Line Data..//
			std::vector<float> LineVertexBuffer;
			std::vector<uint32_t> LineIndexBuffer;
			uint32_t LineIndexCounter = 0;

			//Buffers To Hold Circle Data..//
			std::vector<float> CircleVertexBuffer;
			std::vector<uint32_t> CircleIndexBuffer;
			uint32_t CircleIndexCounter = 0;

			//Ended Declaration Of Buffers..//

			//Get The Main Physics Body From The Components In Question..
			b2Body* body = Component->BodyManagedByTheComponent;

			//Start Getting All Fixtures Associated With The Body..
			for (b2Fixture* f = body->GetFixtureList(); f; f = f->GetNext())
			{
				//Get The Type Of The Shape So That We Can Start Getting The Vertices..
				b2Shape::Type shapetype = f->GetType();

				//Start Extract Vertices From Polygon//
				if (shapetype == b2Shape::e_polygon)
				{
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

						glm::vec2 VertexInPixelSpace = Ermine::vectorWorldToPixels(Vertex);
						glm::vec4 VertexInPixelSpace4 = glm::vec4(VertexInPixelSpace, 0.0f, 0.0f);

						VertexInPixelSpace4 = Component->GetRotationMatrix() * VertexInPixelSpace4;


						TrianglesVertexBuffer.emplace_back(VertexInPixelSpace4.x);
						TrianglesVertexBuffer.emplace_back(VertexInPixelSpace4.y);
						TrianglesVertexBuffer.emplace_back(3.0f);

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
				}
				//Ended Extract Vertices From Polygon//

				//Start Extract Vertices From Edge Shape//
				if(shapetype == b2Shape::e_edge)
				{
					b2EdgeShape* EdgeShape = (b2EdgeShape*)f->GetShape();

					b2Vec2 P1 = EdgeShape->m_vertex1;
					b2Vec2 P2 = EdgeShape->m_vertex2;

					glm::vec2 PixelPoint1 = Ermine::vertexWorldToPixels(P1.x, P1.y);
					glm::vec2 PixelPoint2 = Ermine::vertexWorldToPixels(P2.x, P2.y);

					LineVertexBuffer.emplace_back(PixelPoint1.x);
					LineVertexBuffer.emplace_back(PixelPoint1.y);
					LineVertexBuffer.emplace_back(1.0f);

					LineVertexBuffer.emplace_back(PixelPoint2.x);
					LineVertexBuffer.emplace_back(PixelPoint2.y);
					LineVertexBuffer.emplace_back(1.0f);

					LineIndexBuffer.emplace_back(LineIndexCounter++);
					LineIndexBuffer.emplace_back(LineIndexCounter++);
				}
				//Ended Extract Vertices From Edge Shape//

				//Start Extract Vertices From Chain Shape..//
				if (shapetype == b2Shape::e_chain)
				{
					b2ChainShape* ChainShape = (b2ChainShape*)f->GetShape();
					
					for (int i = 0; i < ChainShape->m_count; i++)
					{
						b2Vec2 Vertex = ChainShape->m_vertices[i];

						glm::vec2 VertexInPixelSpace = Ermine::vertexWorldToPixels(glm::vec2(Vertex.x, Vertex.y));
						//VertexInPixelSpace.y = -1.0f * VertexInPixelSpace.y;

						LineVertexBuffer.emplace_back(VertexInPixelSpace.x);
						LineVertexBuffer.emplace_back(VertexInPixelSpace.y);
						LineVertexBuffer.emplace_back(3.0f);

						LineIndexBuffer.emplace_back(LineIndexCounter++);
					}
				}
				//Ended Extract Vertices From Chain Shape..//

				//Start Extract Vertices To Draw A Circle..//
				if (shapetype == b2Shape::e_circle)
				{
					b2CircleShape* CircleShape = (b2CircleShape*)f->GetShape();

					float radius = Ermine::scalarWorldToPixels(CircleShape->m_radius);
					b2Vec2 OffsetFromCentre = CircleShape->m_p;
					b2Vec2 BodyPos = body->GetPosition();

					BodyPos = BodyPos + OffsetFromCentre;

					glm::vec2 PositionInPixelSpace= Ermine::coordWorldToPixels(glm::vec2(BodyPos.x, BodyPos.y));
					
					//Vertex 0
					CircleVertexBuffer.emplace_back(PositionInPixelSpace.x + radius);
					CircleVertexBuffer.emplace_back(PositionInPixelSpace.y - radius);
					CircleVertexBuffer.emplace_back(1.0f);
					CircleVertexBuffer.emplace_back(radius);
					CircleVertexBuffer.emplace_back((float)Ermine::GetScreenWidth());
					CircleVertexBuffer.emplace_back((float)Ermine::GetScreenHeight());
					CircleVertexBuffer.emplace_back(PositionInPixelSpace.x);
					CircleVertexBuffer.emplace_back(PositionInPixelSpace.y);

					//Vertex 1
					CircleVertexBuffer.emplace_back(PositionInPixelSpace.x + radius);
					CircleVertexBuffer.emplace_back(PositionInPixelSpace.y + radius);
					CircleVertexBuffer.emplace_back(1.0f);
					CircleVertexBuffer.emplace_back(radius);
					CircleVertexBuffer.emplace_back((float)Ermine::GetScreenWidth());
					CircleVertexBuffer.emplace_back((float)Ermine::GetScreenHeight());
					CircleVertexBuffer.emplace_back(PositionInPixelSpace.x);
					CircleVertexBuffer.emplace_back(PositionInPixelSpace.y);

					//Vertex 2
					CircleVertexBuffer.emplace_back(PositionInPixelSpace.x - radius);
					CircleVertexBuffer.emplace_back(PositionInPixelSpace.y + radius);
					CircleVertexBuffer.emplace_back(1.0f);
					CircleVertexBuffer.emplace_back(radius);
					CircleVertexBuffer.emplace_back((float)Ermine::GetScreenWidth());
					CircleVertexBuffer.emplace_back((float)Ermine::GetScreenHeight());
					CircleVertexBuffer.emplace_back(PositionInPixelSpace.x);
					CircleVertexBuffer.emplace_back(PositionInPixelSpace.y);

					//Vertex 3
					CircleVertexBuffer.emplace_back(PositionInPixelSpace.x - radius);
					CircleVertexBuffer.emplace_back(PositionInPixelSpace.y - radius);
					CircleVertexBuffer.emplace_back(1.0f);
					CircleVertexBuffer.emplace_back(radius);
					CircleVertexBuffer.emplace_back((float)Ermine::GetScreenWidth());
					CircleVertexBuffer.emplace_back((float)Ermine::GetScreenHeight());
					CircleVertexBuffer.emplace_back(PositionInPixelSpace.x);
					CircleVertexBuffer.emplace_back(PositionInPixelSpace.y);

					
					std::vector<uint32_t> IndexBuffer = Quad::GetModelIndices();
					
					for (auto& i : IndexBuffer)
						CircleIndexBuffer.emplace_back(i + CircleIndexCounter);
					
					CircleIndexCounter = CircleIndexCounter + 4;
				}
				//Ended Extract Vertices To Draw A Circle..//
			}
			//Populate The Vertex And Index Buffers Of A Body By This Point

			//Start Drawing Everything Using OpenGL Draw Calls//
			VertexArray TriangleVertexArray(TrianglesVertexBuffer, TrianglesIndexBuffer);
			TriangleVertexArray.SetVertexAttribArray(TriangleAndLineVertexArraySpecification);
			TriangleVertexArray.Bind();

			BasicDebuggerShader.Bind();
			BasicDebuggerShader.UniformMat4(std::string("ProjectionViewMatrix"), Renderer->ProjectionViewMatrix);
			BasicDebuggerShader.UniformMat4(std::string("ModelMatrix"), Component->GetTranslationMatrix()); //This Is The Only Reason Why We Are Drawing Per Body.. 
			glDrawElements(GL_TRIANGLES, TriangleVertexArray.GetIndexBufferLength(), GL_UNSIGNED_INT, 0);

			VertexArray LineVertexArray(LineVertexBuffer, LineIndexBuffer);
			LineVertexArray.SetVertexAttribArray(TriangleAndLineVertexArraySpecification);
			LineVertexArray.Bind();

			BasicDebuggerShader.Bind();
			BasicDebuggerShader.UniformMat4(std::string("ProjectionViewMatrix"), Renderer->ProjectionViewMatrix);
			BasicDebuggerShader.UniformMat4(std::string("ModelMatrix"), Component->GetTranslationMatrix()); //This Is The Only Reason Why We Are Drawing Per Body.. 
			glDrawElements(GL_LINE_STRIP, LineVertexArray.GetIndexBufferLength(), GL_UNSIGNED_INT, 0);

			VertexArray CircleVertexArray(CircleVertexBuffer, CircleIndexBuffer);
			CircleVertexArray.SetVertexAttribArray(CircleVertexArraySpecification);
			CircleVertexArray.Bind();

			CircleShader.Bind();
			CircleShader.UniformMat4(std::string("ProjectionViewMatrix"), Renderer->ProjectionViewMatrix);
			CircleShader.UniformMat4(std::string("ModelMatrix"), Component->GetTranslationMatrix()); //This Is The Only Reason Why We Are Drawing Per Body.. 
			glDrawElements(GL_TRIANGLES, CircleVertexArray.GetIndexBufferLength(), GL_UNSIGNED_INT, 0);
			//Ended Drawing Everything Using OpenGL Draw Calls//
		}
	}
}
