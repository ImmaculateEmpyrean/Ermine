#pragma once
#include<iostream>
#include<vector>
#include<string>

#include<filesystem>

#include<mutex>
#include "MutexSystem/Interfaces/IMutex.h"

#include "ImageBase.h"
#include "Physics/Physics.h"
#include "Physics/PhysicsComponent2D.h"

#include "Interfaces/IMovableActor.h"

/*
	Physics Actor2D Extends ImageBase By Implementing The Physics Component In It..
*/

namespace Ermine
{
	class PhysicsActor2D:public Ermine::ImageBase,public Ermine::IMovableActor
	{
	protected:
#pragma region Constructors  
		//Physics Actor Cannot Be Constructed Defaultly As We Need A Physics Component And Also Image Base Cannot Be Constructed Defaultly For Now..
		PhysicsActor2D() = delete;

		//This Constructor Is To Be Used Most of The Time Must Give The Actor With A Physics COmponent..
		PhysicsActor2D(std::shared_ptr<Ermine::Sprite> Spr, std::shared_ptr<PhysicsComponent2D> PhysicsComponent);

	public:
		//Virtual Destructor For The Children Down The Line..
		virtual ~PhysicsActor2D() override;

		//Copy And Move Constructors As Well As Operators Are To Be Overrided So As To Account For The Mutex Inside This Class
	public:
		//Will Implement These In The Future When Copy For Physics Component2D Can Be Implemented..
		PhysicsActor2D(const PhysicsActor2D& rhs) = delete;
		PhysicsActor2D& operator=(const PhysicsActor2D& rhs) = delete;

		PhysicsActor2D(PhysicsActor2D&& rhs);
		PhysicsActor2D& operator=(PhysicsActor2D&& rhs);
#pragma endregion

#pragma region Generators

		//Try Generating The PhysicsActor2D Using One Of These 
	public:
		static std::shared_ptr<Ermine::PhysicsActor2D> Generate(std::filesystem::path TexturePath, std::shared_ptr<PhysicsComponent2D> PhysicsComp);
		static std::shared_ptr<Ermine::PhysicsActor2D> Generate(std::filesystem::path TexturePath, b2BodyDef BodyDef, std::vector<b2FixtureDef> Fixtures);
		static std::shared_ptr<Ermine::PhysicsActor2D> Generate(std::shared_ptr<Ermine::Sprite> sprite, std::shared_ptr<PhysicsComponent2D> PhysicsComp);
		static std::shared_ptr<Ermine::PhysicsActor2D> Generate(std::shared_ptr<Ermine::Sprite> sprite, b2BodyDef BodyDef, std::vector<b2FixtureDef> Fixtures);
#pragma endregion


#pragma region IMovableActorOverrides
		/*Start Overriding Movable Actor Functions*/
		virtual glm::vec2 GetActorPosition() override;
		virtual void SetActorPosition(glm::vec2 ActorPosition) override;

		virtual glm::vec2 GetActorVelocity() override;
		virtual void SetActorVelocity(glm::vec2 ActorVelocity) override;
		
		virtual float GetAngularVelocity(bool Degrees = true) override;
		virtual void  SetAngularVelocity(float AngularVelocity, bool Degrees) override;
		/*Ended Overriding Movable Actor Functions*/
#pragma endregion

#pragma region RenderableGenerationImperatives
//These Functions Are Essential For Interaction With Renderable And Its Implemntation Is Necessary For The Concretization Of Any Class..
		virtual glm::mat4 GetModelMatrix() override;
		virtual std::vector<float> GenerateModelSpaceVertexBuffer() override;
		virtual std::vector<Ermine::VertexAttribPointerSpecification> GetVertexArraySpecification() override;
		virtual std::vector<uint32_t> GenerateModelSpaceIndices() override;
		virtual std::shared_ptr<Ermine::Material> GetAssociatedMaterial() override;
#pragma endregion

#pragma region Actor2DBaseOverrides
		//This Function Returns The Screen Location Of The Object In Question
		virtual glm::vec2 GetScreenLocation() override;

		//This Function Has To Be Overriden In all Children Do Not Forget Otherwise One Child May Be Thought Of As The Other..
		virtual Ermine::ActorFamilyIdentifier GetActorFamilyIdentifier() override { return ActorFamilyIdentifier::PhysicsActor2D; }
#pragma endregion

	public:
		void SetVelocity(glm::vec2 Velocity);
		void SetAngularVelocity(float Velocity);

	public:

	protected:

	protected:

	private:

	private:
		std::shared_ptr<Ermine::PhysicsComponent2D> PhysicsComponent = nullptr;
	};
}