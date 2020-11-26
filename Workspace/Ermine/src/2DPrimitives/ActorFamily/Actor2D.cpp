#include "stdafx.h"
#include "Actor2D.h"

#include "2DPrimitives/Constructs/Quad.h"
#include "2DPrimitives/Constructs/VertexTextured.h"

namespace Ermine
{
#pragma region Constructors
	Actor2D::Actor2D(std::shared_ptr<Sprite> Spr)
		:
		ImageBase(Spr),
		MovableObject()
	{
		auto lock = Object::GetObjectMutex();
		Actor2DConstructionAssciate();
	}
	Actor2D::Actor2D(std::vector<std::shared_ptr<Sprite>> SpriteBuffer)
		:
		ImageBase(std::make_shared<SpriteBook>("SpriteBuffer", SpriteBuffer)),
		MovableObject()
	{
		auto lock = Object::GetObjectMutex();
		Actor2DConstructionAssciate();
	}

	Actor2D::Actor2D(std::shared_ptr<Sprite> Spr, glm::mat4 ModelMatrix)
		:
		ImageBase(Spr),
		MovableObject(ModelMatrix)
	{
		auto lock = Object::GetObjectMutex();
		Actor2DConstructionAssciate();
	}

	Actor2D::Actor2D(std::vector<std::shared_ptr<Sprite>> SpriteBuffer, glm::mat4 ModelMatrix)
		:
		MovableObject(ModelMatrix),
		ImageBase(std::make_shared<SpriteBook>("SpriteBuffer", SpriteBuffer))
	{
		auto lock = Object::GetObjectMutex();
		Actor2DConstructionAssciate();
	}

	Actor2D::~Actor2D()
	{
		RecieveEvents(false, Ermine::EventType::OnUpdateTickEvent);
	}

	//Sorely Missed Copy Constructor
	Actor2D::Actor2D(Actor2D & rhs)
		:
		ImageBase(rhs),
		MovableObject(rhs)
	{
		auto ForeignLock = rhs.GetObjectMutex();
		auto lock = Object::GetObjectMutex();

		Actor2DConstructionAssciate();
	}

	//Copy Operator
	Actor2D& Actor2D::operator=(Actor2D& rhs)
	{
		ImageBase::operator=(rhs);
		MovableObject::operator=(rhs);

		Actor2DConstructionAssciate();

		return *this;
	}

	//Sorely Missed Move Constructor
	Actor2D::Actor2D(Actor2D&& rhs)
		:
		ImageBase(std::move(rhs)),
		MovableObject(std::move(rhs))
	{
		auto ForeignLock = rhs.GetObjectMutex();
		auto lock = Object::GetObjectMutex();

		rhs.SetSprite(nullptr);

		Actor2DConstructionAssciate();
	}

	//Sorely Missed Move Operator 
	Actor2D& Actor2D::operator=(Actor2D&& rhs)
	{
		ImageBase::operator=(std::move(rhs));
		MovableObject::operator=(std::move(rhs));

		Actor2DConstructionAssciate();

		return *this;
	}
#pragma endregion Constructors

#pragma region GeneratorFunctions
	std::shared_ptr<Actor2D> Actor2D::GenerateActor2D(std::filesystem::path TexturePath)
	{
		auto Sprite = GenSprite(TexturePath, glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f));
		std::shared_ptr<Ermine::Actor2D> Act = std::make_shared<Ermine::Actor2D>(Sprite);
		return Act;
	}

	std::shared_ptr<Actor2D> Actor2D::GenerateActor2D(std::filesystem::path TexturePath, glm::vec2 ActorScreenLocation, float Rotation, glm::vec2 Scale)
	{
		glm::mat4 ModelMatrix = GenModelMatrix(ActorScreenLocation, Rotation, Scale);
		auto Spr = GenSprite(TexturePath, glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f));
		std::shared_ptr<Ermine::Actor2D> Act = std::make_shared<Ermine::Actor2D>(Spr,ModelMatrix);
		return Act;
	}

	std::shared_ptr<Actor2D> Actor2D::GenerateActor2D(std::shared_ptr<Sprite> Spr)
	{
		std::shared_ptr<Ermine::Actor2D> Act = std::make_shared<Ermine::Actor2D>(Spr);
		return Act;
	}

	std::shared_ptr<Actor2D> Actor2D::GenerateActor2D(std::shared_ptr<Sprite> Spr, glm::vec2 ActorScreenLocation, float Rotation, glm::vec2 Scale)
	{
		glm::mat4 ModelMatrix = GenModelMatrix(ActorScreenLocation, Rotation, Scale);
		std::shared_ptr<Ermine::Actor2D> Act = std::make_shared<Ermine::Actor2D>(Spr,ModelMatrix);
		return Act;
	}

#pragma endregion

#pragma region MovableActorImplementation
	glm::vec2 Actor2D::GetActorPosition()
	{
		auto lock = Object::GetObjectMutex();
		return MovableObject::GetScreenLocation();
	}
	void Actor2D::SetActorPosition(glm::vec2 ActorPosition)
	{
		auto lock = Object::GetObjectMutex();
		MovableObject::SetPosition(ActorPosition);
	}
	glm::vec2 Actor2D::GetActorVelocity()
	{
		auto lock = Object::GetObjectMutex();
		return MovableObject::GetVelocity();
	}
	void Actor2D::SetActorVelocity(glm::vec2 ActorVelocity)
	{
		auto lock = Object::GetObjectMutex();
		MovableObject::SetVelocity(ActorVelocity);
	}
#pragma endregion MovableActorImplementation

#pragma region HelperGen
	std::shared_ptr<Ermine::Sprite> Actor2D::GenSprite(std::filesystem::path TexturePath, glm::vec2 BottomLeft, glm::vec2 TopRight)
	{
		auto Cache = Ermine::GlobalTextureCache::Get();
		std::shared_ptr<Ermine::Texture> Tex = Cache->GetTextureFromFile(TexturePath);

		std::shared_ptr<Ermine::Sprite> Sprite = Ermine::Sprite::GenerateSprite(Tex,BottomLeft,TopRight);

		return Sprite;
	}

	glm::mat4 Actor2D::GenModelMatrix(glm::vec2 ActorScreenLocation, float Rotation, glm::vec2 Scale)
	{
		glm::mat4 ModelMatrix = glm::mat4(1.0f);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(ActorScreenLocation, 0.0f));
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians<float>(Rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(Scale, 1.0f));

		return ModelMatrix;
	}
#pragma endregion HelperGen


	void Actor2D::Actor2DConstructionAssciate()
	{
		auto Lock = Object::GetObjectMutex();
		RecieveEvents(true, Ermine::EventType::OnUpdateTickEvent);
	}

#pragma region EventProcessing
	void Actor2D::OnUpdateTickEventRecieved()
	{
		auto Lock = Object::GetObjectMutex();

		//Update The Movable Object Every Frame Of The Engine
		MovableObject::Update();

		//MovableObject::Scale(1.01f, 1.01f);
		MovableObject::Translate(1.0f, 0.0f);
		STDOUTDefaultLog_Info("Actor Tick Working..");
	}
#pragma endregion

#pragma region MovableObjectOverrides
	float Actor2D::GetRotation()
	{
		auto Lock = Object::GetObjectMutex();
		return MovableObject::GetRotation();
	}
	float Actor2D::GetAngularVelocity(bool Degrees)
	{
		auto lock = Object::GetObjectMutex();
		return MovableObject::GetAngularVelocity(Degrees);
	}
	glm::vec2 Actor2D::GetScale()
	{
		auto Lock = Object::GetObjectMutex();
		return MovableObject::GetScale();
	}
	void Actor2D::SetPosition(float x, float y)
	{
		auto Lock = Object::GetObjectMutex();
		MovableObject::SetPosition(x, y);
	}
	void Actor2D::SetPosition(glm::vec2 NewPos)
	{
		auto Lock = Object::GetObjectMutex();
		MovableObject::SetPosition(NewPos);
	}
	void Actor2D::Translate(float x, float y)
	{
		auto Lock = Object::GetObjectMutex();
		MovableObject::Translate(x, y);
	}
	void Actor2D::Translate(glm::vec2 TranslateByHowMuch)
	{
		auto Lock = Object::GetObjectMutex();
		MovableObject::Translate(TranslateByHowMuch);
	}
	void Actor2D::ClearTranslations()
	{
		auto Lock = Object::GetObjectMutex();
		MovableObject::ClearTranslations();
	}
	void Actor2D::SetVelocity(float x, float y)
	{
		auto Lock = Object::GetObjectMutex();
		MovableObject::SetVelocity(x, y);
	}
	void Actor2D::SetVelocity(glm::vec2 Velocity)
	{
		auto Lock = Object::GetObjectMutex();
		MovableObject::SetVelocity(Velocity);
	}
	void Actor2D::ClearVelocity()
	{
		auto Lock = Object::GetObjectMutex();
		MovableObject::ClearVelocity();
	}
	void Actor2D::Rotate(float Angle, bool Degrees)
	{
		auto Lock = Object::GetObjectMutex();
		MovableObject::Rotate(Angle, Degrees);
	}
	void Actor2D::SetRotation(float Angle, bool Degrees)
	{
		auto Lock = Object::GetObjectMutex();
		MovableObject::SetRotation(Angle, Degrees);
	}
	void Actor2D::ClearRotations()
	{
		auto Lock = Object::GetObjectMutex();
		MovableObject::ClearRotations();
	}
	void Actor2D::SetAngularVelocity(float AngularVelocity, bool Degrees)
	{
		auto lock = Object::GetObjectMutex();
		MovableObject::SetAngularVelocity(AngularVelocity, Degrees);
	}
	void Actor2D::ClearAngularVelocity()
	{
		auto Lock = Object::GetObjectMutex();
		MovableObject::ClearAngularVelocity();
	}
	void Actor2D::SetScale(float x, float y)
	{
		auto Lock = Object::GetObjectMutex();
		MovableObject::SetScale(x, y);
	}
	void Actor2D::SetScale(glm::vec2 Scale)
	{
		auto Lock = Object::GetObjectMutex();
		MovableObject::SetScale(Scale);
	}
	void Actor2D::Scale(float x, float y)
	{
		auto Lock = Object::GetObjectMutex();
		MovableObject::Scale(x, y);
	}
	void Actor2D::Scale(glm::vec2 ScaleByHowMuch)
	{
		auto Lock = Object::GetObjectMutex();
		MovableObject::Scale(ScaleByHowMuch);
	}
	void Actor2D::ClearScale()
	{
		auto Lock = Object::GetObjectMutex();
		MovableObject::ClearScale();
	}
	glm::mat4 Actor2D::GetModelMatrix()
	{
		auto Lock = Object::GetObjectMutex();
		return MovableObject::GetModelMatrix();
	}
	glm::mat4 Actor2D::GetTranslationMatrix()
	{
		auto Lock = Object::GetObjectMutex();
		return MovableObject::GetTranslationMatrix();
	}
	glm::mat4 Actor2D::GetRotationMatrix()
	{
		auto Lock = Object::GetObjectMutex();
		return MovableObject::GetRotationMatrix();
	}
	glm::mat4 Actor2D::GetScaleMatrix()
	{
		auto Lock = Object::GetObjectMutex();
		return MovableObject::GetScaleMatrix();
	}
	glm::vec2 Actor2D::GetScreenLocation()
	{
		auto Lock = Object::GetObjectMutex();
		return MovableObject::GetScreenLocation();
	}
	glm::vec2 Actor2D::GetVelocity()
	{
		auto Lock = Object::GetObjectMutex();
		return MovableObject::GetVelocity();
	}
#pragma endregion

#pragma region RenderavleGenerationImperatives
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
	std::vector<Ermine::VertexAttribPointerSpecification> Actor2D::GetVertexArraySpecification()
	{
		return {
					{3,GL_FLOAT,false},
					{3,GL_FLOAT,false},
					{2,GL_FLOAT,false},
					{1,GL_FLOAT,false}
			   };
	}

	std::vector<uint32_t> Actor2D::GenerateModelSpaceIndices()
	{
		return Quad::GetModelIndices();
	}


	std::shared_ptr<Ermine::Material> Actor2D::GetAssociatedMaterial()
	{
		//The Image Base Must Have Default Setted Required Material.. :>
		return Actor2DBase::GetMaterial();
	}
#pragma endregion
}