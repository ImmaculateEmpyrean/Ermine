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
	class PhysicsActor:public Ermine::ImageBase,public Ermine::IMovableActor
	{
	public:
#pragma region Constructors  
		//Physics Actor Cannot Be Constructed Defaultly As We Need A Physics Component And Also Image Base Cannot Be Constructed Defaultly For Now..
		PhysicsActor() = delete;

		//This Constructor Is To Be Used Most of The Time Must Give The Actor With A Physics COmponent..
		PhysicsActor(std::shared_ptr<Ermine::Sprite> Spr, PhysicsComponent2D Phys);

		//Virtual Destructor For The Children Down The Line..
		virtual ~PhysicsActor() override;

		//Copy And Move Constructors As Well As Operators Are To Be Overrided So As To Account For The Mutex Inside This Class
	public:
		//Will Implement These In The Future When Copy For Physics Component2D Can Be Implemented..
		PhysicsActor(const PhysicsActor& rhs) = delete;
		PhysicsActor& operator=(const PhysicsActor& rhs) = delete;

		PhysicsActor(PhysicsActor&& rhs);
		PhysicsActor& operator=(PhysicsActor&& rhs);
#pragma endregion

#pragma region Generators

		//Try Generating The PhysicsActor2D Using One Of These 
	public:
		static std::shared_ptr<Ermine::PhysicsActor2D> Generate(std::filesystem::path TexturePath, std::shared_ptr<PhysicsComponent2D> PhysicsComp);
		static std::shared_ptr<Ermine::PhysicsActor2D> Generate(std::filesystem::path TexturePath, b2BodyDef BodyDef, std::vector<b2FixtureDef> Fixtures);
		static std::shared_ptr<Ermine::PhysicsActor2D> Generate(std::shared_ptr<Ermine::Sprite>, std::shared_ptr<PhysicsComponent2D> PhysicsComp);
		static std::shared_ptr<Ermine::PhysicsActor2D> Generate(std::shared_ptr<Ermine::Sprite>, b2BodyDef BodyDef, std::vector<b2FixtureDef> Fixtures);
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
//This Function Is Essential For Interaction With Renderable And Its Implemntation Is Necessary For The Concretization Of Any Class..
		virtual glm::mat4 GetModelMatrix();
		virtual std::vector<float> GenerateModelSpaceVertexBuffer();
		virtual std::vector<Ermine::VertexAttribPointerSpecification> GetVertexArraySpecification();
		virtual std::vector<uint32_t> GenerateModelSpaceIndices();
		virtual std::shared_ptr<Ermine::Material> GetAssociatedMaterial();

		std::shared_ptr<Ermine::Material> GetMaterial();
		void SetMaterial(std::shared_ptr<Ermine::Material> Mat);
#pragma endregion

	public:
		//This Function Returns The Screen Location Of The Object In Question
		virtual glm::vec2 GetScreenLocation() override;

		void SetVelocity(glm::vec2 Velocity);
		void SetAngularVelocity(float Velocity);

		virtual std::vector<float> CalculateModelSpaceVertexes(); //This Used To Be An Override.. Instead Remove This Function In Refactor.. Rendering Is No Longer Handled By The Actor Or Its Thread..

		//This Function Has To Be Overriden In all Children Do Not Forget Otherwise One Child May Be Thought Of As The Other..
		virtual Ermine::ActorFamilyIdentifier GetActorFamilyIdentifier() override { return ActorFamilyIdentifier::PhysicsActor2D; }

	public:

	protected:

	protected:

	private:
#pragma region Helpers
		void HelperMove(PhysicsActor&& rhs);
#pragma endregion

	private:
		std::shared_ptr<Ermine::PhysicsComponent2D> PhysicsComponent = nullptr;

	};
}