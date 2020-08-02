#include "stdafx.h"
#include "Actor2D.h"

#include "glm.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Graphics/Renderer/RendererPrimitives/VertexArray.h"

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


	std::vector<float> Actor2D::GetModelSpaceCoordinates()
	{
		std::vector<float> ModelCoordinates = Quad::GetModelCoordinates();

		//Top Right..
		ModelCoordinates[6] = Actorsprite->GetTopRightUV().x;
		ModelCoordinates[7] = Actorsprite->GetBottomLeftUV().y;//Actorsprite->GetTopRightUV().y;

		//Bottom Right
		ModelCoordinates[15] = Actorsprite->GetTopRightUV().x;//ModelCoordinates[14] = Actorsprite->GetBottomLeftUV().x;
		ModelCoordinates[16] = Actorsprite->GetTopRightUV().y;//Actorsprite->GetBottomLeftUV().y;

		//Bottom Left
		ModelCoordinates[24] = Actorsprite->GetBottomLeftUV().x;
		ModelCoordinates[25] = Actorsprite->GetTopRightUV().y;//Actorsprite->GetBottomLeftUV().y;

		//Top Left
		ModelCoordinates[33] = Actorsprite->GetBottomLeftUV().x;
		ModelCoordinates[34] = Actorsprite->GetBottomLeftUV().y;//Actorsprite->GetTopRightUV().y;

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
