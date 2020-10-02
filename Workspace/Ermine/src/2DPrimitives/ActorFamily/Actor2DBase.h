#pragma once
#include<iostream>
#include<string>
#include<vector>

#include "2DPrimitives/PrimitiveType2D.h"
#include "ActorFamilyEnnumerations.h"

#include "Graphics/Renderer/RendererPrimitives/VertexArray.h" //This Need Not Be Included Here I Think I Can Getby with A Simple Forward Declaration But Whatever..

namespace Ermine {

	//An Actor Is Something That Is Displayed And Movable Nothing Else.. Do Not Asume It Contains Only one Sprite Or Any Such Nonesense..
	//The Actor Class Is Always Described With Respect To Center Of The Quad..

	//The Class Actor2DBase As Of Now Is expected To do Nothing And Hence It Is Left Open.. In Thhe Future It May Be Populated..

	class Actor2DBase
	{
	public:
		//A Class which Holds Nothing Need Not Be Constructed In a Specialized Way Unless Absolutely Required..
		Actor2DBase() = default;

		//A Virtual Destructor For The Children Which May Actually Manage Data Members..
		virtual ~Actor2DBase();

	public:

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

	protected:

	protected:

	private:

	private:
	

	};

}