#include "stdafx.h"
#include "PhysicsActor2D.h"

#include "2DPrimitives/Constructs/Quad.h"
#include "2DPrimitives/Constructs/VertexTextured.h"

#include "Physics/Physics.h"

#include "Graphics/Renderer/Renderer2D.h"

namespace Ermine
{

#pragma region Constructors
	PhysicsActor2D::PhysicsActor2D(std::shared_ptr<Ermine::Sprite> Spr, std::shared_ptr<PhysicsComponent2D> PhysicsComponent)
		:
		ImageBase(Spr),
		PhysicsComponent(PhysicsComponent)
	{}
	PhysicsActor2D::~PhysicsActor2D()
	{}

	PhysicsActor2D::PhysicsActor2D(PhysicsActor2D&& rhs)
		:
		ImageBase(std::move(rhs))
	{
		auto ForeignLock = rhs.GetObjectMutex();
		auto Lock = GetObjectMutex();
		
		PhysicsComponent = std::move(rhs.PhysicsComponent);
	}
	PhysicsActor2D& PhysicsActor2D::operator=(PhysicsActor2D&& rhs)
	{
		auto ForeignLock = rhs.GetObjectMutex();
		auto Lock = GetObjectMutex();

		ImageBase::operator=(std::move(rhs));
		PhysicsComponent = std::move(rhs.PhysicsComponent);

		return *this;
	}
	
#pragma endregion Constructors

	std::shared_ptr<Ermine::PhysicsActor2D> PhysicsActor2D::Generate(std::filesystem::path TexturePath, std::shared_ptr<PhysicsComponent2D> PhysicsComp)
	{
		std::shared_ptr<Ermine::PhysicsActor2D> Actor(new Ermine::PhysicsActor2D(Ermine::Sprite::GenerateSprite(TexturePath), PhysicsComp));
		return Actor;
	}
	std::shared_ptr<Ermine::PhysicsActor2D> PhysicsActor2D::Generate(std::filesystem::path TexturePath, b2BodyDef BodyDef, std::vector<b2FixtureDef> Fixtures)
	{
		auto Component = Ermine::PhysicsComponent2D::Generate(BodyDef, Fixtures);
		auto Sprite = Ermine::Sprite::GenerateSprite(TexturePath);

		std::shared_ptr<Ermine::PhysicsActor2D> Actor(new Ermine::PhysicsActor2D(Sprite, Component));
		return Actor;
	}
	std::shared_ptr<Ermine::PhysicsActor2D> PhysicsActor2D::Generate(std::shared_ptr<Ermine::Sprite> sprite, std::shared_ptr<PhysicsComponent2D> PhysicsComp)
	{
		std::shared_ptr<Ermine::PhysicsActor2D> Actor(new Ermine::PhysicsActor2D(sprite, PhysicsComp));
		return Actor;
	}
	std::shared_ptr<Ermine::PhysicsActor2D> PhysicsActor2D::Generate(std::shared_ptr<Ermine::Sprite> sprite, b2BodyDef BodyDef, std::vector<b2FixtureDef> Fixtures)
	{
		auto Component = Ermine::PhysicsComponent2D::Generate(BodyDef, Fixtures);

		std::shared_ptr<Ermine::PhysicsActor2D> Actor(new Ermine::PhysicsActor2D(sprite, Component));
		return Actor;
	}

	void PhysicsActor2D::InitiateDebugRendering()
	{
		/*if(PhysicsComponent != nullptr)
			Ermine::Renderer2D::SubmitPhysicsComponent2D(PhysicsComponent)*/
	}

	void PhysicsActor2D::DeInitiateDebugRendering()
	{
	}

#pragma region Generator

#pragma endregion

	//Movable Actor Overrides//
	glm::vec2 PhysicsActor2D::GetActorPosition()
	{
		//The Physics Actor Also Derives From The Object Type..
		auto Lock = GetObjectMutex();

		return PhysicsComponent->GetBodyLocationPixelSpace();
	}

	void PhysicsActor2D::SetActorPosition(glm::vec2 ActorPosition)
	{
		//The Physics Actor Also Derives From The Object Type..
		auto Lock = GetObjectMutex();

		PhysicsComponent->SetPosition(ActorPosition);
	}

	glm::vec2 PhysicsActor2D::GetActorVelocity()
	{
		//The Physics Actor Also Derives From The Object Type..
		auto Lock = GetObjectMutex();

		return PhysicsComponent->GetVelocityOfTheBody();
	}

	void PhysicsActor2D::SetActorVelocity(glm::vec2 ActorVelocity)
	{
		//The Physics Actor Also Derives From The Object Type..
		auto Lock = GetObjectMutex();

		PhysicsComponent->SetVelocity(ActorVelocity);
	}

	float PhysicsActor2D::GetAngularVelocity(bool Degrees)
	{
		//The Physics Actor Also Derives From The Object Type..
		auto Lock = GetObjectMutex();

		if(Degrees == true)
			return glm::degrees<float>(PhysicsComponent->GetAngularVelocityOfTheBody());
		else return PhysicsComponent->GetAngularVelocityOfTheBody();
	}

	void PhysicsActor2D::SetAngularVelocity(float AngularVelocity, bool Degrees)
	{
		//The Physics Actor Also Derives From The Object Type..
		auto Lock = GetObjectMutex();

		if (Degrees == true)
			AngularVelocity = glm::radians<float>(AngularVelocity);

		PhysicsComponent->SetAngularVelocity(AngularVelocity);
	}

	//Ended Movable Overrides//


	glm::vec2 PhysicsActor2D::GetScreenLocation()
	{
		//The Physics Actor Also Derives From The Object Type..
		auto Lock = GetObjectMutex();

		return PhysicsComponent->GetBodyLocationPixelSpace();
	}

	void PhysicsActor2D::SetVelocity(glm::vec2 Velocity)
	{
		//The Physics Actor Also Derives From The Object Type..
		auto Lock = GetObjectMutex();

		PhysicsComponent->SetVelocity(Velocity);
	}
	void PhysicsActor2D::SetAngularVelocity(float Velocity)
	{
		//The Physics Actor Also Derives From The Object Type..
		auto Lock = GetObjectMutex();

		PhysicsComponent->SetAngularVelocity(Velocity);
	}

#pragma region RenderableGenerationImperatives

	glm::mat4 PhysicsActor2D::GetModelMatrix()
	{
		//The Physics Actor Also Derives From The Object Type..
		auto Lock = GetObjectMutex();

		return PhysicsComponent->GetModelMatrix();
	}
	std::vector<float> PhysicsActor2D::GenerateModelSpaceVertexBuffer()
	{
		//The Physics Actor Also Derives From The Object Type..
		auto Lock = GetObjectMutex();

		Ermine::VertexTextured TopRight(Quad::GetModelCoordinatesTopRight());
		Ermine::VertexTextured BottomRight(Quad::GetModelCoordinatesBottomRight());
		Ermine::VertexTextured BottomLeft(Quad::GetModelCoordinatesBottomLeft());
		Ermine::VertexTextured TopLeft(Quad::GetModelCoordinatesTopLeft());

		//NOTE- The vectorWorldToErmine() PArtly Works Because We Never Pass In A Negative Value.. This Function Is Not To Translate The Points On The Graph 

		glm::vec2 TopRightPixelCalculate = glm::vec2(PhysicsComponent->GetBoundingBoxWidth()/2.0f, PhysicsComponent->GetBoundingBoxHeight()/2.0f);		//Ermine::vectorWorldToErmine(glm::vec2(GetBodyWidthBox2DSpace() / 2.0f, GetBodyHeightBox2DSpace() / 2));										//glm::vec2 TopRightPixelCalculate = Ermine::coordWorldToErmine(glm::vec2(GetBodyWidth() / 2.0f, GetBodyHeight() / 2));
		glm::vec3 TopRightPos = glm::vec3(TopRightPixelCalculate.x, TopRightPixelCalculate.y, 0.0f);
		glm::vec4 TopRightPos4 = glm::vec4(TopRightPos, 0.0f);

		glm::vec2 BottomRightPixelCalculate = glm::vec2(PhysicsComponent->GetBoundingBoxWidth()/2.0f, -1.0f * (PhysicsComponent->GetBoundingBoxHeight()/2.0f));
		glm::vec3 BottomRightPos = glm::vec3(BottomRightPixelCalculate.x, BottomRightPixelCalculate.y, 0.0f);
		glm::vec4 BottomRightPos4 = glm::vec4(BottomRightPos, 0.0f);

		glm::vec2 BottomLeftPixelCalculate = glm::vec2(-1.0f * (PhysicsComponent->GetBoundingBoxWidth()/2.0f), -1.0f * (PhysicsComponent->GetBoundingBoxHeight()/2.0f));
		glm::vec3 BottomLeftPos = glm::vec3(BottomLeftPixelCalculate.x, BottomLeftPixelCalculate.y, 0.0f);
		glm::vec4 BottomLeftPos4 = glm::vec4(BottomLeftPos, 0.0f);

		glm::vec2 TopLeftPixelCalculate = glm::vec2(-1.0f * (PhysicsComponent->GetBoundingBoxWidth()/2.0f), (PhysicsComponent->GetBoundingBoxHeight()/2.0f));
		glm::vec3 TopLeftPos = glm::vec3(TopLeftPixelCalculate.x, TopLeftPixelCalculate.y, 0.0f);//TopLeft.GetPositionCoordinates();
		glm::vec4 TopLeftPos4 = glm::vec4(TopLeftPos, 0.0f);

		TopRight.SetPositonCoordinates(TopRightPos4);
		BottomRight.SetPositonCoordinates(BottomRightPos4);
		BottomLeft.SetPositonCoordinates(BottomLeftPos4);
		TopLeft.SetPositonCoordinates(TopLeftPos4);

		std::shared_ptr<Ermine::Sprite> Actorsprite = GetSprite();
		TopRight.SetVertexUV(glm::vec2(Actorsprite->GetTopRightUV().x, Actorsprite->GetBottomLeftUV().y));
		BottomRight.SetVertexUV(glm::vec2(Actorsprite->GetTopRightUV().x, Actorsprite->GetTopRightUV().y));
		BottomLeft.SetVertexUV(glm::vec2(Actorsprite->GetBottomLeftUV().x, Actorsprite->GetTopRightUV().y));
		TopLeft.SetVertexUV(glm::vec2(Actorsprite->GetBottomLeftUV().x, Actorsprite->GetBottomLeftUV().y));

		std::vector<float> ModelCoordinates;
		ModelCoordinates = TopRight;
		ModelCoordinates = ModelCoordinates + BottomRight;
		ModelCoordinates = ModelCoordinates + BottomLeft;
		ModelCoordinates = ModelCoordinates + TopLeft;

		return ModelCoordinates;
	}
	std::vector<Ermine::VertexAttribPointerSpecification> PhysicsActor2D::GetVertexArraySpecification()
	{
		return {
					{3,GL_FLOAT,false},
					{3,GL_FLOAT,false},
					{2,GL_FLOAT,false},
					{1,GL_FLOAT,false}
		};
	}
	std::vector<uint32_t> PhysicsActor2D::GenerateModelSpaceIndices()
	{
		return Quad::GetModelIndices();
	}
	
#pragma endregion
}