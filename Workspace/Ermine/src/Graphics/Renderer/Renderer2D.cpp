#include "stdafx.h"
#include "Renderer2D.h"

#include "RendererCommands.h"

#include "Graphics/Renderer/MaterialSystem/Material.h"
#include "Graphics/Renderer/MaterialSystem/Texture.h"
#include "EngineResourceHandlers/GlobalTextureCache.h"

#include "RendererPrimitives/VertexBuffer.h"
#include "RendererPrimitives/IndexBuffer.h"

#include "2DPrimitives/Constructs/VertexTextured.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "OpenGLErrorChecker.h"

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
	

	struct RendererStruct
	{
		unsigned int vertex_array = 0;
		glm::mat4 ProjectionMatrix = glm::mat4(1.0f);
		glm::mat4 ViewMatrix = glm::mat4(1.0f);

		uint32_t CurrentVertices = 0;

		unsigned int TextureBoundSlot = 1;

		std::shared_ptr<Ermine::Material> TexturedQuadMaterial;

		std::shared_ptr<Ermine::VertexBuffer> Vbo;
		std::shared_ptr<Ermine::IndexBuffer>  Ibo;
	};

	static RendererStruct Def;
	static Ermine::Renderer2D::Statistics Stats;

	void Renderer2D::Init()
	{
		GLCall(glGenVertexArrays(1, &Def.vertex_array));
		GLCall(glBindVertexArray(Def.vertex_array));

		Ermine::VertexLayout Layout;
		Layout.AddSpecification(3, GL_FLOAT); //Vertex Position..
		Layout.AddSpecification(4, GL_FLOAT); //Vertex Color..
		Layout.AddSpecification(2, GL_FLOAT); //Vertex UV..
		Layout.AddSpecification(1, GL_INT);   //Texture Number..

		Layout.UpdateSpecification();

		//Setup The Material..
		Def.TexturedQuadMaterial = Ermine::Material::Generate(std::filesystem::path("Shader/Vertex/TexturedQuad.vert"), std::filesystem::path("Shader/Fragment/TexturedQuad.frag"));

		//A Horrid Magenta Serves As The Clear Color.. So That One Does Not Mistake It For Something Else.. X>
		RendererCommands::SetClearColor(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
	}
	void Renderer2D::Shutdown()
	{
		if (Def.vertex_array != 0)
			GLCall(glDeleteVertexArrays(1, &Def.vertex_array));
	}


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
		RendererCommands::DrawIndexed(Def.Vbo,Def.Ibo, Def.TexturedQuadMaterial);
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

		Def.Vbo->AppendVertex(BottomLeft.GetVertex());
		Def.Vbo->AppendVertex(TopLeft.GetVertex());
		Def.Vbo->AppendVertex(TopRight.GetVertex());
		Def.Vbo->AppendVertex(BottomRight.GetVertex());

		Def.Ibo->AddIndex(0);
		Def.Ibo->AddIndex(1);
		Def.Ibo->AddIndex(2);
		Def.Ibo->AddIndex(2);
		Def.Ibo->AddIndex(3);
		Def.Ibo->AddIndex(0);
	}
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const std::shared_ptr<Ermine::Sprite>& Sprite)
	{
		DrawQuad(glm::vec3(position.x, position.y, 0.0f), size, Sprite);
	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const std::shared_ptr<Ermine::Sprite>& Sprite)
	{
		Ermine::VertexTextured BottomLeft;
		Ermine::VertexTextured TopLeft;
		Ermine::VertexTextured TopRight;
		Ermine::VertexTextured BottomRight;

		float WidthRadius = size.x / 2.0f;
		float HeightRadius = size.y / 2.0f;

		if (++Def.TextureBoundSlot > MaxTextureSlots)
			Flush();

		std::shared_ptr<Ermine::Texture> Tex = Sprite->GetTexture();
		Tex->Bind(Def.TextureBoundSlot);

		BottomLeft.SetPositionCoordinates(position.x - WidthRadius, position.y - HeightRadius);
		BottomLeft.SetVertexUV(Sprite->GetBottomLeftUV());
		BottomLeft.SetTextureNumber((float)Def.TextureBoundSlot);

		TopLeft.SetPositionCoordinates(position.x - WidthRadius, position.y + HeightRadius);
		TopLeft.SetVertexUV(glm::vec2(Sprite->GetBottomLeftUV().x,Sprite->GetTopRightUV().y));
		TopLeft.SetTextureNumber((float)Def.TextureBoundSlot);

		TopRight.SetPositionCoordinates(position.x + WidthRadius, position.y + HeightRadius);
		TopRight.SetVertexUV(Sprite->GetTopRightUV());
		TopRight.SetTextureNumber((float)Def.TextureBoundSlot);

		BottomRight.SetPositionCoordinates(position.x + WidthRadius, position.y - HeightRadius);
		BottomRight.SetVertexUV(glm::vec2(Sprite->GetTopRightUV().x,Sprite->GetBottomLeftUV().y));
		BottomRight.SetTextureNumber((float)Def.TextureBoundSlot);

		std::vector<float> Vertices = BottomLeft + TopLeft + TopRight + BottomRight;
		Vao->PushVertices(Vertices);
		Def.CurrentVertices += 4;
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