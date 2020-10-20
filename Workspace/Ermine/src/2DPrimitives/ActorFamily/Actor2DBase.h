#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<mutex>

#include "2DPrimitives/PrimitiveType2D.h"
#include "ActorFamilyEnnumerations.h"

#include "Graphics/Renderer/RendererPrimitives/VertexArray.h" //This Need Not Be Included Here I Think I Can Getby with A Simple Forward Declaration But Whatever..

#include "EventSystem/SubscriptionTicket.h"
#include "EventSystem/EventBroadcastStation.h"

namespace Ermine {

	//An Actor Is Something That Is Displayed And Movable Nothing Else.. Do Not Asume It Contains Only one Sprite Or Any Such Nonesense..
	//The Actor Class Is Always Described With Respect To Center Of The Quad..

	//The Class Actor2DBase As Of Now Is expected To do Nothing And Hence It Is Left Open.. In Thhe Future It May Be Populated..

	class Actor2DBase
	{
	public:
		//A Class which Holds Nothing Exposed To The Outside Readily Need Not Be Constructed In a Specialized Way Unless Absolutely Required..
		Actor2DBase();

		//A Virtual Destructor For The Children Which May Actually Manage Data Members..
		virtual ~Actor2DBase();

	public:
		//Copy Must Be Custom Constructed As To Account For The Ticket
		Actor2DBase(const Actor2DBase& rhs);
		Actor2DBase operator=(const Actor2DBase& rhs) = delete; //This Class Can Never Be Instantiated.. What Point In This Operator
		
		//Move Must Be Custom Constructed As To Account For The Ticket
		Actor2DBase(Actor2DBase&& rhs);
		Actor2DBase operator=(Actor2DBase&& rhs) = delete; //This Class Can Never Be Instantiated.. What Point In This Operator

	public:
		//This Function Has To Be Overriden In all Children Do Not Forget Otherwise One Child May Be Thought Of As The Other..
		virtual Ermine::ActorFamilyIdentifier GetActorFamilyIdentifier() { return ActorFamilyIdentifier::Actor2DBase; }

		//This Function Is Used To Recognize The Family 
		Ermine::PrimitiveType2D GetFamilyIdentifier() { return PrimitiveType2D::ACTOR2D; }

		//This Function Is Used To Get The ModelSpace Indices.. This Is Different If The Actor is a Quad As Opposed To a PolyLine Hence It Cannot Be Implemnted Here.. 
		virtual std::vector<uint32_t> GetIndices() = 0;

		//This Function Is Also Vital And Also One That Cannot Be Implemented Here As I Donot Know What Shape The Actor Is In..
		virtual std::vector<float> CalculateModelSpaceVertexes() = 0;

		//This Function Is Used To Know What Specification The Vertex Array Has To Be Set In.. However This Cannot Be known Now As We Know Nothing About The Vertexes That Make Up Our Actor..
		virtual std::vector<VertexAttribPointerSpecification> GetVertexAttribSpecificationForTheActor() = 0;

		//This Function Is Used To Get The Centre Of Any Actor On Screen.. 
		virtual glm::vec2 GetScreenLocation() = 0;

		//This Function Simply Returns An Object That LOcks The Mutex Simply Let It Be Destroyed After You Are Done To Unlock The Mutex..
		std::unique_lock<std::recursive_mutex>&& GetActorStandradMutex();

		
		//This Tick Can Be Overriden By Any Child.. And Maybe Used To Set Something In Relation To The Entire Class On A Tick.. i.e This Function Will Most Definitely Be Called On All Instances Of Actor and its virtual so it will u know be dispatched to the proper class 
		virtual void ClassOnTick(float DeltaTime) { return; }

		//This Is An Event.. Assign Some Function Here If You Wanna Tick..
		std::function<void(float)>& OnTick() { return OnTickFunction; }

	protected:

	protected:

	private:
		void OnTickActorBase(Event* Message);

		void HelperConstructActorBase();

	private:
		//This Mutex Must Be Claimed When Performing Anything On Actor.. Since Actor NOw Is Multithreaded.. 
		std::recursive_mutex ActorStandradMutex;

		std::atomic_bool ActorReadyToRecieveEvents = true;
		Ermine::SubscriptionTicket* OnTickEventTicket = nullptr;

		//This Is The Function Which Will Be Run By The Actor As The Event OnTick.. Assign A Function Of Your Choice To Be Run
		std::function<void(float)> OnTickFunction = nullptr;
	};

}