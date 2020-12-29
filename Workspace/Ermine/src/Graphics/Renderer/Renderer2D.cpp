#include "stdafx.h"
#include "Renderer2D.h"

#include "RendererCommands.h"

#include "Graphics/Renderer/MaterialSystem/Material.h"
#include "Graphics/Renderer/MaterialSystem/Texture.h"

#include "RendererPrimitives/VertexArray.h"
#include "RendererPrimitives/VertexBuffer.h"
#include "RendererPrimitives/IndexBuffer.h"

#include "2DPrimitives/Constructs/VertexTextured.h"

//TODO- Remove This really this need not be here.. get rid of it..
#include "App.h"
#include "Level/Scene.h"

#include "EntityComponentSystem/Entity.h"

namespace Ermine {
	//Renderer Defaults
	static constexpr uint32_t MaxQuads = 25000;
	static constexpr uint32_t MaxVertices = MaxQuads * 4;
	static constexpr uint32_t MaxIndices = MaxQuads * 6;
	static constexpr uint32_t MaxTextureSlots = 32;
	

	std::shared_ptr<Ermine::VertexArray> Vao;
	std::shared_ptr<Ermine::Material>	 TexturedQuadMaterial;

	struct RendererStruct
	{
		glm::mat4 ProjectionMatrix = glm::mat4(1.0f);
		glm::mat4 ViewMatrix = glm::mat4(1.0f);

		uint32_t CurrentVertices = 0;
	};

	static RendererStruct Def;
	static Ermine::Renderer2D::Statistics Stats;

	void Renderer2D::Init()
	{
		//Start Setup The Vertex Array Object..
		Vao = Ermine::VertexArray::Generate();

		Ermine::VertexSpecifics VertexSpecs;
		VertexSpecs.AddSpecification(3, GL_FLOAT); //Vertex Position..
		VertexSpecs.AddSpecification(4, GL_FLOAT); //Vertex Color..
		VertexSpecs.AddSpecification(2, GL_FLOAT); //Vertex UV..
		VertexSpecs.AddSpecification(1, GL_FLOAT); //Texture Number..

		Vao->SetVertexAttribArray(VertexSpecs);

		//Setup The Material..
		TexturedQuadMaterial = Ermine::Material::Generate(std::filesystem::path("Shader/Vertex/TexturedQuad.vert"), std::filesystem::path("Shader/Fragment/TexturedQuad.frag"));

		//A Horrid Magenta Serves As The Clear Color.. So That One Does Not Mistake It For Something Else.. X>
		RendererCommands::SetClearColor(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
	}
	void Renderer2D::Shutdown()
	{}


	void Renderer2D::BeginScene()
	{
		RendererCommands::InitializeNewFrame();

		auto Scene = Ermine::GetHandleToApp()->GetScene();
		Ermine::Entity* CameraEntity = Scene->GetPrimaryCamera();

		//Get The View Matrix For Translating The Matrix..
		Ermine::TransformComponent TransComp = CameraEntity->GetComponent<Ermine::TransformComponent>();
		Def.ViewMatrix = TransComp.Transform;

		//Get The Projection Matrix.. To Be Used..
		Ermine::CameraComponent CameraComp = CameraEntity->GetComponent<CameraComponent>();
		Def.ProjectionMatrix = CameraComp.Projection;
	}

	void Renderer2D::Flush()
	{
		RendererCommands::DrawIndexed(Vao, TexturedQuadMaterial);
	}

	void Renderer2D::EndScene()
	{
		Flush();
		RendererCommands::FlushFrame();

		//Clear The Statistics For Next Frame..
		Stats.DrawCalls = 0;
		Stats.QuadCount = 0;
	}

	
#pragma region DrawingQuadFunctions

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad(glm::vec3(position.x, position.y, 0.0f), size, color);
	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		/*std::vector<float> VertexBuffer;

		//Vertex 1.. BottomLeft
		VertexBuffer.emplace_back(position.x - (size.x / 2.0f));
		VertexBuffer.emplace_back(position.y - (size.y / 2.0f));
		VertexBuffer.emplace_back(0.0f);

		VertexBuffer.emplace_back(color.r);
		VertexBuffer.emplace_back(color.g);
		VertexBuffer.emplace_back(color.b);
		VertexBuffer.emplace_back(color.a);

		VertexBuffer.emplace_back(0.0f);
		VertexBuffer.emplace_back(0.0f);

		VertexBuffer.emplace_back(33.0f);

		//Vertex 2.. TopLeft
		VertexBuffer.emplace_back(position.x - (size.x / 2.0f));
		VertexBuffer.emplace_back(position.y + (size.y / 2.0f));
		VertexBuffer.emplace_back(0.0f);

		VertexBuffer.emplace_back(color.r);
		VertexBuffer.emplace_back(color.g);
		VertexBuffer.emplace_back(color.b);
		VertexBuffer.emplace_back(color.a);

		VertexBuffer.emplace_back(0.0f);
		VertexBuffer.emplace_back(0.0f);

		VertexBuffer.emplace_back(33.0f);

		//Vertex 3.. TopRight
		VertexBuffer.emplace_back(position.x + (size.x / 2.0f));
		VertexBuffer.emplace_back(position.y + (size.y / 2.0f));
		VertexBuffer.emplace_back(0.0f);

		VertexBuffer.emplace_back(color.r);
		VertexBuffer.emplace_back(color.g);
		VertexBuffer.emplace_back(color.b);
		VertexBuffer.emplace_back(color.a);

		VertexBuffer.emplace_back(0.0f);
		VertexBuffer.emplace_back(0.0f);

		VertexBuffer.emplace_back(33.0f);

		//Vertex 4.. BottomRight
		VertexBuffer.emplace_back(position.x + (size.x / 2.0f));
		VertexBuffer.emplace_back(position.y - (size.y / 2.0f));
		VertexBuffer.emplace_back(0.0f);

		VertexBuffer.emplace_back(color.r);
		VertexBuffer.emplace_back(color.g);
		VertexBuffer.emplace_back(color.b);
		VertexBuffer.emplace_back(color.a);

		VertexBuffer.emplace_back(0.0f);
		VertexBuffer.emplace_back(0.0f);

		VertexBuffer.emplace_back(33.0f);
		
		//Add The Vertices Into The Vertex Array..
		Vao->PushVertices(VertexBuffer);

		//Set The Indices So As to Draw The Said Quad..
		std::vector<uint32_t> IndexBuffer;

		IndexBuffer.emplace_back(Def.CurrentVertices);
		IndexBuffer.emplace_back(Def.CurrentVertices + 1);
		IndexBuffer.emplace_back(Def.CurrentVertices + 2);

		IndexBuffer.emplace_back(Def.CurrentVertices + 2);
		IndexBuffer.emplace_back(Def.CurrentVertices + 3);
		IndexBuffer.emplace_back(Def.CurrentVertices);

		Vao->PushIndices(IndexBuffer);

		//Update The Current Vertices..
		Def.CurrentVertices = Def.CurrentVertices + 4;*/

		Ermine::VertexTextured BottomLeft;
		Ermine::VertexTextured TopLeft;
		Ermine::VertexTextured TopRight;
		Ermine::VertexTextured BottomRight;

		float WidthRadius = size.x / 2.0f;
		float HeightRadius = size.y / 2.0f;

		BottomLeft.SetPositionCoordinates(position.x - WidthRadius, position.y - HeightRadius);
		BottomLeft.SetVertexColorValue(color);
		
		TopLeft.SetPositionCoordinates(position.x - WidthRadius, position.y + HeightRadius);
		TopLeft.SetVertexColorValue(color);

		TopRight.SetPositionCoordinates(position.x + WidthRadius, position.y + HeightRadius);
		TopRight.SetVertexColorValue(color);

		BottomRight.SetPositionCoordinates(position.x + WidthRadius, position.y - HeightRadius);
		BottomRight.SetVertexColorValue(color);


	}
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const std::shared_ptr<Ermine::Sprite>& Sprite)
	{
		DrawQuad(glm::vec3(position.x, position.y, 0.0f), size, Sprite);
	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const std::shared_ptr<Ermine::Sprite>& Sprite)
	{
		
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
	{
		Ermine::Entity Ent = Ermine::Entity(Ermine::GetHandleToApp()->GetScene());

		//Add The Transform Component..
		Ent.AddComponent<Ermine::TransformComponent>(transform);

		//Add The Color Component Right..
		std::vector<glm::vec4()> VertexColors;
		VertexColors.emplace_back(glm::vec4(color.r, color.g, color.b, color.a));
		Ent.AddComponent<Ermine::VertexTintComponent>(VertexColors);
	}
	void Renderer2D::DrawQuad(const glm::mat4& transform, const std::shared_ptr<Ermine::Sprite>& Spr)
	{
		Ermine::Entity Ent = Ermine::Entity(Ermine::GetHandleToApp()->GetScene());

		//Add The Transform Component..
		Ent.AddComponent<Ermine::TransformComponent>(transform);

		//Add The SpriteRender Component..
		Ent.AddComponent<Ermine::SpriteRenderComponent>(Spr);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawRotatedQuad(glm::vec3(position, 0.0f), size, rotation, color);
	}
	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{

	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const std::shared_ptr<Ermine::Sprite>& Sprite)
	{
		DrawRotatedQuad(glm::vec3(position,0.0f), size, rotation, Sprite);
	}
	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const std::shared_ptr<Ermine::Sprite>& Sprite)
	{

	}


#pragma endregion
}