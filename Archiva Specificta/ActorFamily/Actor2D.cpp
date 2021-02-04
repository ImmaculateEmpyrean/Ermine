#include "stdafx.h"
#include "Actor2D.h"

#include "2DPrimitives/Constructs/Quad.h"
#include "2DPrimitives/Constructs/VertexTextured.h"

namespace Ermine
{
#pragma region Constructors
	Actor2D::Actor2D()
		:
		Actor2DShape()
	{}

	Actor2D::Actor2D(std::vector<std::shared_ptr<Sprite>> SpriteBuffer, glm::mat4 ModelMatrix)
		:
		Actor2DShape(SpriteBuffer,ModelMatrix,std::vector<VertexTextured>())
	{}

	Actor2D::~Actor2D()
	{}

	//Sorely Missed Copy Constructor
	Actor2D::Actor2D(Actor2D & rhs)
		:
		Actor2DShape(rhs)
	{}

	//Copy Operator
	Actor2D& Actor2D::operator=(Actor2D& rhs)
	{
		auto ForeignLock = rhs.GetObjectMutex();
		auto Lock = GetObjectMutex();

		Actor2DShape::operator=(rhs);
		return *this;
	}

	//Sorely Missed Move Constructor
	Actor2D::Actor2D(Actor2D&& rhs)
		:
		Actor2DShape(std::move(rhs))
	{}

	//Sorely Missed Move Operator 
	Actor2D& Actor2D::operator=(Actor2D&& rhs)
	{
		auto ForeignLock = rhs.GetObjectMutex();
		auto Lock = GetObjectMutex();

		Actor2DShape::operator=(std::move(rhs));
		return *this;
	}
#pragma endregion Constructors

#pragma region GeneratorFunctions
	std::shared_ptr<Actor2D> Actor2D::GenerateActor2D(std::filesystem::path TexturePath)
	{
		glm::mat4 ModelMatrix = GenModelMatrix({ 0.0f,0.0f }, 0.0f, { 1.0f,1.0f });
		auto Sprite = ImageBase::GenSprite(TexturePath, glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f));
		std::vector<std::shared_ptr<Ermine::Sprite>> Sprites;
		Sprites.emplace_back(Sprite);

		std::shared_ptr<Ermine::Actor2D> Act(new Actor2D(Sprites, ModelMatrix));
		return Act;
	}

	std::shared_ptr<Actor2D> Actor2D::GenerateActor2D(std::filesystem::path TexturePath, glm::vec2 ActorScreenLocation, float Rotation, glm::vec2 Scale)
	{
		glm::mat4 ModelMatrix = GenModelMatrix(ActorScreenLocation, Rotation, Scale);
		auto Spr = ImageBase::GenSprite(TexturePath, glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f));
		std::vector<std::shared_ptr<Ermine::Sprite>> Sprites;
		Sprites.emplace_back(Spr);

		std::shared_ptr<Ermine::Actor2D> Act(new Actor2D(Sprites, ModelMatrix));
		return Act;
	}

	std::shared_ptr<Actor2D> Actor2D::GenerateActor2D(std::shared_ptr<Sprite> Spr)
	{
		glm::mat4 ModelMatrix = GenModelMatrix({ 0.0f,0.0f }, 0.0f, {1.0f,1.0f});
		std::vector<std::shared_ptr<Ermine::Sprite>> Sprites;
		Sprites.emplace_back(Spr);

		std::shared_ptr<Ermine::Actor2D> Act(new Actor2D(Sprites, ModelMatrix));

		return Act;
	}

	std::shared_ptr<Actor2D> Actor2D::GenerateActor2D(std::shared_ptr<Sprite> Spr, glm::vec2 ActorScreenLocation, float Rotation, glm::vec2 Scale)
	{
		glm::mat4 ModelMatrix = GenModelMatrix(ActorScreenLocation, Rotation, Scale);
		std::vector<std::shared_ptr<Ermine::Sprite>> Sprites;
		Sprites.emplace_back(Spr);

		std::shared_ptr<Ermine::Actor2D> Act(new Actor2D(Sprites, ModelMatrix));
		
		return Act;
	}

#pragma endregion



#pragma region HelperGen
	

	glm::mat4 Actor2D::GenModelMatrix(glm::vec2 ActorScreenLocation, float Rotation, glm::vec2 Scale)
	{
		glm::mat4 ModelMatrix = glm::mat4(1.0f);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(ActorScreenLocation, 0.0f));
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians<float>(Rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(Scale, 1.0f));

		return ModelMatrix;
	}
#pragma endregion HelperGen

#pragma region RendreableGenerationImperatives
	std::vector<float> Actor2D::GenerateModelSpaceVertexBuffer()
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

		TopRight.SetPositonCoordinates(TopRightPos4);
		BottomRight.SetPositonCoordinates(BottomRightPos4);
		BottomLeft.SetPositonCoordinates(BottomLeftPos4);
		TopLeft.SetPositonCoordinates(TopLeftPos4);

		//Get The Mutex As We Are Starting To Use Shared Memory
		auto Lock = GetObjectMutex();

		TopRight.SetVertexUV(   glm::vec2(GetTopRightUV().x  ,GetBottomLeftUV().y));
		BottomRight.SetVertexUV(glm::vec2(GetTopRightUV().x  ,GetTopRightUV().y  ));
		BottomLeft.SetVertexUV( glm::vec2(GetBottomLeftUV().x,GetTopRightUV().y  ));
		TopLeft.SetVertexUV(    glm::vec2(GetBottomLeftUV().x,GetBottomLeftUV().y));

		//We Are Done Using Shared Resources..
		Lock.unlock();

		std::vector<float> ModelCoordinates;
		ModelCoordinates = TopRight;
		ModelCoordinates = ModelCoordinates + BottomRight;
		ModelCoordinates = ModelCoordinates + BottomLeft;
		ModelCoordinates = ModelCoordinates + TopLeft;

		return ModelCoordinates;
	}
	

	std::vector<uint32_t> Actor2D::GenerateModelSpaceIndices()
	{
		return Quad::GetModelIndices();
	}


	int32_t Actor2D::GetRendererDrawMode()
	{
		return GL_TRIANGLES;
	}
#pragma endregion
}