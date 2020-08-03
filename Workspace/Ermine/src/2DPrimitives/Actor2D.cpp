#include "stdafx.h"
#include "Actor2D.h"

#include "glm.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Graphics/Renderer/RendererPrimitives/VertexArray.h"

#include "Constructs/VertexBase.h"
#include "Constructs/VertexTextured.h"

namespace Ermine
{
	Actor2D::Actor2D(std::shared_ptr<Sprite> Spr)
		:
		Actorsprite(Spr)
	{
		HelperInitializeRenderable2D();
	}
	Actor2D::Actor2D(std::vector<std::shared_ptr<Sprite>> SpriteBuffer)
	{
		Actorsprite = std::make_shared<SpriteBook>("SpriteBuffer",SpriteBuffer);
		HelperInitializeRenderable2D();
	}

	Actor2D::Actor2D(std::shared_ptr<Sprite> Spr, glm::mat4 ModelMatrix)
		:
		MovableObject(ModelMatrix),
		Actorsprite(Spr)
	{
		HelperInitializeRenderable2D();
	}

	Actor2D::Actor2D(std::vector<std::shared_ptr<Sprite>> SpriteBuffer, glm::mat4 ModelMatrix)
		:
		MovableObject(ModelMatrix)
	{
		Actorsprite = std::make_shared<SpriteBook>("SpriteBuffer",SpriteBuffer);
		HelperInitializeRenderable2D();
	}

	Actor2D::~Actor2D()
	{}


	glm::mat4 Actor2D::GetModelMatrix()
	{
		return MovableObject::RecievedModelMatrix  * MovableObject::TranslationMatrix;
	}

	std::vector<float> Actor2D::GetModelSpaceCoordinates()
	{
		Ermine::VertexTextured TopRight(Quad::GetModelCoordinatesTopRight());
		Ermine::VertexTextured BottomRight(Quad::GetModelCoordinatesBottomRight());
		Ermine::VertexTextured BottomLeft(Quad::GetModelCoordinatesBottomLeft());
		Ermine::VertexTextured TopLeft(Quad::GetModelCoordinatesTopLeft());

		glm::vec3 TopRightPos = TopRight.GetPositionCoordinates();
		glm::vec4 TopRightPos4 = glm::vec4(TopRightPos, 0.0f);

		glm::vec3 BottomRightPos = BottomRight.GetPositionCoordinates();
		glm::vec4 BottomRightPos4 = glm::vec4(BottomRightPos, 0.0f);

		glm::vec3 BottomLeftPos = BottomLeft.GetPositionCoordinates();
		glm::vec4 BottomLeftPos4 = glm::vec4(BottomLeftPos, 0.0f);

		glm::vec3 TopLeftPos = TopLeft.GetPositionCoordinates();
		glm::vec4 TopLeftPos4 = glm::vec4(TopLeftPos, 0.0f);

		TopRightPos4    = RotationMatrix * TopRightPos4;
		BottomRightPos4 = RotationMatrix * BottomRightPos4;
		BottomLeftPos4  = RotationMatrix * BottomLeftPos4;
		TopLeftPos4     = RotationMatrix * TopLeftPos4;

		TopRightPos4    = ScaleMatrix * TopRightPos4;
		BottomRightPos4 = ScaleMatrix * BottomRightPos4;
		BottomLeftPos4  = ScaleMatrix * BottomLeftPos4;
		TopLeftPos4     = ScaleMatrix * TopLeftPos4;
		
		TopRight.SetPositonCoordinates(TopRightPos4);
		BottomRight.SetPositonCoordinates(BottomRightPos4);
		BottomLeft.SetPositonCoordinates(BottomLeftPos4);
		TopLeft.SetPositonCoordinates(TopLeftPos4);

		TopRight.SetVertexUV(glm::vec2(Actorsprite->GetTopRightUV().x, Actorsprite->GetBottomLeftUV().y));
		BottomRight.SetVertexUV(glm::vec2(Actorsprite->GetTopRightUV().x, Actorsprite->GetTopRightUV().y));
		BottomLeft.SetVertexUV(glm::vec2(Actorsprite->GetBottomLeftUV().x, Actorsprite->GetTopRightUV().y));
		TopLeft.SetVertexUV(glm::vec2(Actorsprite->GetBottomLeftUV().x, Actorsprite->GetBottomLeftUV().y));

		std::vector<float> ModelCoordinates;
		ModelCoordinates = TopRight;
		ModelCoordinates = ModelCoordinates + BottomRight;
		ModelCoordinates = ModelCoordinates + BottomLeft;
		ModelCoordinates = ModelCoordinates + TopLeft;

		//std::vector<float> ModelCoordinates = Quad::GetModelCoordinates();

		//Top Right..
		/*ModelCoordinates[6] = Actorsprite->GetTopRightUV().x;
		ModelCoordinates[7] = Actorsprite->GetBottomLeftUV().y;//Actorsprite->GetTopRightUV().y;

		//Bottom Right
		ModelCoordinates[15] = Actorsprite->GetTopRightUV().x;//ModelCoordinates[14] = Actorsprite->GetBottomLeftUV().x;
		ModelCoordinates[16] = Actorsprite->GetTopRightUV().y;//Actorsprite->GetBottomLeftUV().y;

		//Bottom Left
		ModelCoordinates[24] = Actorsprite->GetBottomLeftUV().x;
		ModelCoordinates[25] = Actorsprite->GetTopRightUV().y;//Actorsprite->GetBottomLeftUV().y;

		//Top Left
		ModelCoordinates[33] = Actorsprite->GetBottomLeftUV().x;
		ModelCoordinates[34] = Actorsprite->GetBottomLeftUV().y;//Actorsprite->GetTopRightUV().y;*/

		return ModelCoordinates;
	}

	std::vector<uint32_t> Actor2D::GetModelSpaceIndices()
	{
		return Quad::GetModelIndices();
	}

	std::shared_ptr<Sprite> Actor2D::GetSprite()
	{
		return Actorsprite;
	}
	void Actor2D::SetSprite(std::shared_ptr<Sprite> Sprite)
	{
		Actorsprite = Sprite;
		RenderableTextureModule::Clear();
		RenderableTextureModule::SubmitTexture(Actorsprite->GetTexture());
	}


	void Actor2D::HelperInitializeRenderable2D()
	{
		auto Vao = Ermine::VertexArray(VertexBuffer(GetModelSpaceCoordinates()), IndexBuffer(GetModelSpaceIndices()));
		static std::vector<VertexAttribPointerSpecification> Spec = {
				{3,GL_FLOAT,false},
				{3,GL_FLOAT,false},
				{2,GL_FLOAT,false},
				{1,GL_FLOAT,false}
		};
		Vao.SetVertexAttribArray(Spec);
		
		Renderable2D::SetVertexArray(std::move(Vao));
		Renderable2D::SetMaterial(Ermine::Material(std::filesystem::path("Shader/Vertex/Actor2DUpdatedWithRenderableTextureModuleVertexShader.vert"),
												   std::filesystem::path("Shader/Fragment/Actor2DUpdatedWithRenderableTextureModuleFragmentShader.frag")));
		//Renderable2D::SetMaterial(Ermine::Material(std::filesystem::path("Shader/Actor2DBaseMaterial.json")));
		RenderableTextureModule::SubmitTexture(Actorsprite->GetTexture());
	}
	std::vector<int> Actor2D::BindTexturesContained()
	{
		std::vector<int> BoundVector;
		BoundVector.resize(16, 0);

		auto TextureCacheGlobal = Ermine::GlobalTextureCache::Get();

		int BoundSlot = TextureCacheGlobal->Bind(Actorsprite->GetTexture());
		BoundVector[0] = BoundSlot;
		
		//This Totally Must Not Be Here.. And Also Consider Creating a New COmponent Which Allows U to Set Uv's At Will..
		HelperInitializeRenderable2D();

		return BoundVector;
	}
}
