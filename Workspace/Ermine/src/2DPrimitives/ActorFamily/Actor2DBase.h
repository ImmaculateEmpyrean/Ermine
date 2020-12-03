#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<mutex>

#include "Object.h"
#include "MutexSystem/Interfaces/IMutex.h"

#include "2DPrimitives/PrimitiveType2D.h"
#include "ActorFamilyEnnumerations.h"

#include "Graphics/Renderer/RendererPrimitives/VertexArray.h" //This Need Not Be Included Here I Think I Can Getby with A Simple Forward Declaration But Whatever..

#include "EventSystem/SubscriptionTicket.h"
#include "EventSystem/EventBroadcastStation.h"

#include "Graphics/Renderer/MaterialSystem/Material.h"

namespace Ermine {

	//An Actor Is Something That Is Displayed And Movable Nothing Else.. Do Not Asume It Contains Only one Sprite Or Any Such Nonesense..
	//The Actor Class Is Always Described With Respect To Center Of The Quad..

	//The Class Actor2DBase As Of Now Is expected To do Nothing And Hence It Is Left Open.. In Thhe Future It May Be Populated..

	class Actor2DBase : public Ermine::Object
	{
	public:
		//A Class which Holds Nothing Exposed To The Outside Readily Need Not Be Constructed In a Specialized Way Unless Absolutely Required..
		Actor2DBase();

		Actor2DBase(std::shared_ptr<Ermine::Material> Mat);

		//A Virtual Destructor For The Children Which May Actually Manage Data Members..
		virtual ~Actor2DBase();

	public:
		//Copy Must Be Custom Constructed As To Account For The Ticket
		Actor2DBase(Actor2DBase& rhs);
		Actor2DBase& operator=(Actor2DBase& rhs);
		
		//Move Must Be Custom Constructed As To Account For The Ticket
		Actor2DBase(Actor2DBase&& rhs);
		Actor2DBase& operator=(Actor2DBase&& rhs);

	public:
		//This Function Has To Be Overriden In all Children Do Not Forget Otherwise One Child May Be Thought Of As The Other..
		virtual Ermine::ActorFamilyIdentifier GetActorFamilyIdentifier() { return ActorFamilyIdentifier::Actor2DBase; }

		//This Function Is Used To Recognize The Family 
		Ermine::PrimitiveType2D GetFamilyIdentifier() { return PrimitiveType2D::ACTOR2D; }


#pragma region VirtualInterfaceFunctions
		//This Function Is Used To Get The Centre Of Any Actor On Screen.. 
		virtual glm::vec2 GetScreenLocation() = 0;
#pragma endregion

#pragma region RenderableGenerationImperatives
		//This Function Is Essential For Interaction With Renderable And Its Implemntation Is Necessary For The Concretization Of Any Class..
		virtual glm::mat4 GetModelMatrix() = 0;
		virtual std::vector<float> GenerateModelSpaceVertexBuffer() = 0;
		virtual std::vector<Ermine::VertexAttribPointerSpecification> GetVertexArraySpecification() = 0;
		virtual std::vector<uint32_t> GenerateModelSpaceIndices() = 0;
		
#pragma endregion

		//Start Interact With The Material Of Actor2DBase
		std::shared_ptr<Ermine::Material> GetMaterial();
		void SetMaterial(std::shared_ptr<Ermine::Material> Mat);
		//Ended Interact With The Material Of Actor2DBase

		//Test
		virtual void OnUpdateTickEventRecieved() override;
	

	protected:

	protected:

	private:

	private:
		std::shared_ptr<Ermine::Material> ActorMaterial = nullptr;
	};

}