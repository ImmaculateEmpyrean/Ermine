#include "stdafx.h"
#include "PhysicsActor2D.h"

#include "2DPrimitives/Constructs/Quad.h"
#include "2DPrimitives/Constructs/VertexTextured.h"

#include "Physics/Physics.h"

namespace Ermine
{

#pragma region Constructors
	PhysicsActor::PhysicsActor(std::shared_ptr<Ermine::Sprite> Spr)
		:
		ImageBase(Spr)
	{}

	PhysicsActor::PhysicsActor(std::shared_ptr<Ermine::Sprite> Spr, PhysicsComponent2D Phys)
		:
		ImageBase(Spr),
		PhysicsComponent2D(std::move(Phys))
	{}

	PhysicsActor::~PhysicsActor()
	{
		//This Is Not Used As Of Now.. A Formality So That Virtual Dispatches Properly..
	}

	PhysicsActor::PhysicsActor(PhysicsActor&& rhs)
		:
		ImageBase(std::move(rhs)),
		PhysicsComponent2D(std::move(rhs))
	{}

	PhysicsActor& PhysicsActor::operator=(PhysicsActor&& rhs)
	{
		auto ForeignLock = rhs.GetObjectMutex();
		auto Lock = GetObjectMutex();

		ImageBase::operator=(std::move(rhs));
		PhysicsComponent2D::operator=(std::move(rhs));

		return *this;
	}
#pragma endregion Constructors

	//Movable Actor Overrides//
	glm::vec2 PhysicsActor::GetActorPosition()
	{
		//The Physics Actor Also Derives From The Object Type..
		auto Lock = GetObjectMutex();

		return PhysicsComponent2D::GetBodyLocationPixelSpace();
	}

	void PhysicsActor::SetActorPosition(glm::vec2 ActorPosition)
	{
		//The Physics Actor Also Derives From The Object Type..
		auto Lock = GetObjectMutex();

		PhysicsComponent2D::SetPosition(ActorPosition);
	}

	glm::vec2 PhysicsActor::GetActorVelocity()
	{
		//The Physics Actor Also Derives From The Object Type..
		auto Lock = GetObjectMutex();

		return PhysicsComponent2D::GetVelocityOfTheBody();
	}

	void PhysicsActor::SetActorVelocity(glm::vec2 ActorVelocity)
	{
		//The Physics Actor Also Derives From The Object Type..
		auto Lock = GetObjectMutex();

		PhysicsComponent2D::SetVelocity(ActorVelocity);
	}

	float PhysicsActor::GetAngularVelocity(bool Degrees)
	{
		//The Physics Actor Also Derives From The Object Type..
		auto Lock = GetObjectMutex();

		if(Degrees == true)
			return glm::degrees<float>(PhysicsComponent2D::GetAngularVelocityOfTheBody());
		else return PhysicsComponent2D::GetAngularVelocityOfTheBody();
	}

	void PhysicsActor::SetAngularVelocity(float AngularVelocity, bool Degrees)
	{
		//The Physics Actor Also Derives From The Object Type..
		auto Lock = GetObjectMutex();

		if (Degrees == true)
			AngularVelocity = glm::radians<float>(AngularVelocity);

		PhysicsComponent2D::SetAngularVelocity(AngularVelocity);
	}

	//Ended Movable Overrides//


	glm::vec2 PhysicsActor::GetScreenLocation()
	{
		//The Physics Actor Also Derives From The Object Type..
		auto Lock = GetObjectMutex();

		return PhysicsComponent2D::GetBodyLocationPixelSpace();
	}

	void PhysicsActor::SetVelocity(glm::vec2 Velocity)
	{
		//The Physics Actor Also Derives From The Object Type..
		auto Lock = GetObjectMutex();

		PhysicsComponent2D::SetVelocity(Velocity);
	}
	void PhysicsActor::SetAngularVelocity(float Velocity)
	{
		//The Physics Actor Also Derives From The Object Type..
		auto Lock = GetObjectMutex();

		PhysicsComponent2D::SetAngularVelocity(Velocity);
	}

#pragma region RenderableGenerationImperatives

	glm::mat4 PhysicsActor::GetModelMatrix()
	{
		//The Physics Actor Also Derives From The Object Type..
		auto Lock = GetObjectMutex();

		return PhysicsComponent2D::GetModelMatrix();
	}
	std::vector<float> PhysicsActor::GenerateModelSpaceVertexBuffer()
	{
		//The Physics Actor Also Derives From The Object Type..
		auto Lock = GetObjectMutex();

		Ermine::VertexTextured TopRight(Quad::GetModelCoordinatesTopRight());
		Ermine::VertexTextured BottomRight(Quad::GetModelCoordinatesBottomRight());
		Ermine::VertexTextured BottomLeft(Quad::GetModelCoordinatesBottomLeft());
		Ermine::VertexTextured TopLeft(Quad::GetModelCoordinatesTopLeft());

		//NOTE- The vectorWorldToErmine() PArtly Works Because We Never Pass In A Negative Value.. This Function Is Not To Translate The Points On The Graph 

		glm::vec2 TopRightPixelCalculate = Ermine::vectorWorldToErmine(glm::vec2(GetBodyWidthBox2DSpace() / 2.0f, GetBodyHeightBox2DSpace() / 2));//glm::vec2 TopRightPixelCalculate = Ermine::coordWorldToErmine(glm::vec2(GetBodyWidth() / 2.0f, GetBodyHeight() / 2));
		glm::vec3 TopRightPos = glm::vec3(TopRightPixelCalculate.x, TopRightPixelCalculate.y, 0.0f);
		glm::vec4 TopRightPos4 = glm::vec4(TopRightPos, 0.0f);

		glm::vec2 BottomRightPixelCalculate = Ermine::vectorWorldToErmine(glm::vec2(GetBodyWidthBox2DSpace() / 2.0f, -1.0f * (GetBodyHeightBox2DSpace() / 2)));
		glm::vec3 BottomRightPos = glm::vec3(BottomRightPixelCalculate.x, BottomRightPixelCalculate.y, 0.0f);
		glm::vec4 BottomRightPos4 = glm::vec4(BottomRightPos, 0.0f);

		glm::vec2 BottomLeftPixelCalculate = Ermine::vectorWorldToErmine(glm::vec2(-1.0f * (GetBodyWidthBox2DSpace() / 2.0f), -1.0f * (GetBodyHeightBox2DSpace() / 2)));
		glm::vec3 BottomLeftPos = glm::vec3(BottomLeftPixelCalculate.x, BottomLeftPixelCalculate.y, 0.0f);
		glm::vec4 BottomLeftPos4 = glm::vec4(BottomLeftPos, 0.0f);

		glm::vec2 TopLeftPixelCalculate = Ermine::vectorWorldToErmine(glm::vec2(-1.0f * (GetBodyWidthBox2DSpace() / 2.0f), (GetBodyHeightBox2DSpace() / 2)));
		glm::vec3 TopLeftPos = glm::vec3(TopLeftPixelCalculate.x, TopLeftPixelCalculate.y, 0.0f);//TopLeft.GetPositionCoordinates();
		glm::vec4 TopLeftPos4 = glm::vec4(TopLeftPos, 0.0f);

		//Start Get Rotation Matrix For This Physics Actor..//
		glm::mat4 RotationMatrix = PhysicsComponent2D::GetRotationMatrix();
		//Ended Get Rotation Matrix For This Physics Actor..//

		/*Start This I Guess Is The Only Difference Between Movable And Non Movable Actor */

		TopRightPos4 = RotationMatrix * TopRightPos4;
		BottomRightPos4 = RotationMatrix * BottomRightPos4;
		BottomLeftPos4 = RotationMatrix * BottomLeftPos4;
		TopLeftPos4 = RotationMatrix * TopLeftPos4;

		/*Ended This I Guess Is The Only Difference Between Movable And Non Movable Actor */

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
	std::vector<Ermine::VertexAttribPointerSpecification> PhysicsActor::GetVertexArraySpecification()
	{
		return {
					{3,GL_FLOAT,false},
					{3,GL_FLOAT,false},
					{2,GL_FLOAT,false},
					{1,GL_FLOAT,false}
		};
	}
	std::vector<uint32_t> PhysicsActor::GenerateModelSpaceIndices()
	{
		return Quad::GetModelIndices();
	}
	std::shared_ptr<Ermine::Material> PhysicsActor::GetAssociatedMaterial()
	{
		//The Physics Actor Also Derives From The Object Type..
		auto Lock = GetObjectMutex();

		return Actor2DBase::GetMaterial();
	}
	std::shared_ptr<Ermine::Material> PhysicsActor::GetMaterial()
	{
		//The Physics Actor Also Derives From The Object Type..
		auto Lock = GetObjectMutex();

		return std::shared_ptr<Ermine::Material>();
	}
	void PhysicsActor::SetMaterial(std::shared_ptr<Ermine::Material> Mat)
	{
		//The Physics Actor Also Derives From The Object Type..
		auto Lock = GetObjectMutex();

		Actor2DBase::SetMaterial(Mat);
	}
#pragma endregion
}