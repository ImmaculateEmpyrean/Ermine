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

		//Create The Shader Once And Keep It To Be Used In The Future..
		static Ermine::Shader PhyCompShader(std::filesystem::path("Shader/Vertex/PhysicsDebuggerVertexShader.vert"),
											std::filesystem::path("Shader/Fragment/PhysicsDebuggerFragmentShader.frag"));
		
		for (auto Component : PhysicsComponentsBuffer)
		{
			b2Body* body = Component->BodyManagedByTheComponent;

			//Start Getting All Fixtures Associated With The Body..
			int FixtureCount = 0;
			for (b2Fixture* f = body->GetFixtureList(); f ;f =f->GetNext())
			{
				//Get The Type Of The Shape So That We Can Start Getting The Vertices..
				b2Shape::Type shapetype = f->GetType();

				if (shapetype == b2Shape::e_polygon)
				{
					//Store All The Vertexes To Draw Here..
					std::vector<float> VertexBuffer;
					//Store All The Indexes Here..
					std::vector<uint32_t> IndexBuffer;
					uint32_t IndexCounter = 0;

					b2PolygonShape* PolygonShape = (b2PolygonShape*)f->GetShape();
					
					for (int i = 0; i < PolygonShape->m_count; i++)
					{
						b2Vec2 BodyPostionB2Vec2 = body->GetPosition();
						glm::vec2 BodyPosition = glm::vec2(BodyPostionB2Vec2.x, BodyPostionB2Vec2.y);

						glm::vec2 Vertex = glm::vec2(PolygonShape->m_vertices[i].x, PolygonShape->m_vertices[i].y); //This Is Returning Garbage Fix It..
						
						Vertex = Vertex; //+ BodyPosition;

						glm::vec2 VertexInPixelSpace = Ermine::vectorWorldToPixels(Vertex);
						glm::vec4 VertexInPixelSpace4 = glm::vec4(VertexInPixelSpace, 0.0f,0.0f);

						VertexInPixelSpace4 = Component->GetRotationMatrix()* VertexInPixelSpace4;


						VertexBuffer.emplace_back(VertexInPixelSpace4.x);
						VertexBuffer.emplace_back(VertexInPixelSpace4.y);
						VertexBuffer.emplace_back(3.0f);

						//Add A New Index Into The Index Buffer
						IndexCounter++;
						//IndexBuffer.emplace_back(IndexCounter++);
						//IndexBuffer.emplace_back(IndexCounter);
					}
					int Sc = 1;
					int Tr = 2;
					for (int i = 0; Tr < IndexCounter; i++)
					{
						IndexBuffer.emplace_back(0);
						IndexBuffer.emplace_back(Sc);
						IndexBuffer.emplace_back(Tr);

						Sc++;
						Tr++;
					}
					//IndexBuffer[IndexBuffer.size() - 1] = 0;

					VertexArray Vao(VertexBuffer, IndexBuffer);
					static std::vector<VertexAttribPointerSpecification> Spec = {
						   {3,GL_FLOAT,false}
					};

					Vao.SetVertexAttribArray(Spec);
					Vao.Bind();

					PhyCompShader.Bind();

					//PhyCompShader.Uniform4f(std::string("InFragColor"), glm::vec4(255.0f,255.0f,255.0f,1.0f));
					PhyCompShader.UniformMat4(std::string("ProjectionViewMatrix"), Renderer->ProjectionViewMatrix);
					PhyCompShader.UniformMat4(std::string("ModelMatrix"), Component->GetTranslationMatrix());
					glDrawElements(GL_TRIANGLES, Vao.GetIndexBufferLength(), GL_UNSIGNED_INT, 0);
				}

				if (shapetype == b2Shape::e_circle)
				{
					//Start Get The Shader Ready To Be Drawn//
					static Ermine::Shader Shd(std::filesystem::path("Shader/Vertex/PhysicsDebuggerDrawCircle.vert"), std::filesystem::path("Shader/Fragment/PhysicsDebuggerDrawCircle.frag"));
					Shd.Bind();
					//Shd.UniformMat4(std::string("ProjectionViewMatrix"), Renderer->ProjectionViewMatrix);
					Shd.Uniform2f(std::string("u_resolution"), glm::vec2(1000.0f, 1000.0f));
					Shd.Bind();
					//Ended Get The Shader Ready To Be Drawn//

					b2CircleShape* CircleShape = (b2CircleShape*)f->GetShape();
					
					float radius = CircleShape->m_radius;
					b2Vec2 OffsetFromCentre = CircleShape->m_p;

					std::vector<uint32_t> IndexBuffer = Quad::GetModelIndices();
					
					std::vector<float> VertexBuffer{ 
						// positions    
					 0.5f,  0.5f, 0.0f, 
					 0.5f, -0.5f, 0.0f, 
					-0.5f, -0.5f, 0.0f, 
					-0.5f,  0.5f, 0.0f
					};

					VertexArray Vaof(VertexBuffer, IndexBuffer);
					Vaof.Bind();
					
					static std::vector<VertexAttribPointerSpecification> Spec = {
						{3,GL_FLOAT,false}
					};

					Vaof.SetVertexAttribArray(Spec);
					Vaof.Bind();

					glDrawElements(GL_TRIANGLES, Vaof.GetIndexBufferLength(), GL_UNSIGNED_INT, 0);
				}
			}
		}
	}
}
