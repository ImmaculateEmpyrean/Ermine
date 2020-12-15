#pragma once
#include<iostream>
#include<vector>
#include<string>

#include "glm.hpp"

#include "2DPrimitives/Constructs/MovableObject.h"

#include "Interfaces/IMovableActor.h"
#include "ShapeBase.h"

namespace Ermine
{
	class Actor2DShape: public MovableObject, public ShapeBase
	{
	public:
#pragma region Constructors

		//Well You Can Totally Have An Empty Shape.. Just Not An Default One..
		Actor2DShape();

		//This Is Useful Now And Then
		Actor2DShape(std::vector<std::shared_ptr<Sprite>> SpriteBuffer, glm::mat4 ModelMatrix,std::vector<VertexTextured> Points);

		//Start Copy And Move Constructor Are Custom Implemented.. For Now Just Because..
		Actor2DShape(Actor2DShape& rhs);
		Actor2DShape& operator=(Actor2DShape& rhs);

		Actor2DShape(Actor2DShape&& rhs);
		Actor2DShape& operator=(Actor2DShape&& rhs);
		//Ended Copy And Move Constructor Are Custom Implemented.. For Now Just Because..

		//The Destructor Has To Be Implemented Probably
		virtual ~Actor2DShape();

	public:
		//Generate Function..
		//std::shared_ptr<Ermine::Actor2DShape> Generate(glm::vec2 SpawnLocation, std::vector<VertexTextured> Vertices, std::shared_ptr<Ermine::Sprite> Spr);
#pragma endregion

#pragma region Actor2DBaseOverrides
	public:
		//virtual glm::mat4 GetModelMatrix() override;
#pragma endregion

	public:
#pragma region Actor2DBaseImplementation
		//virtual glm::vec2 GetScreenLocation() { return GetActorPosition(); };
#pragma endregion



#pragma region MovableActorOverrides
		virtual glm::mat4 GetModelMatrix()								  override;
		virtual glm::mat4 GetTranslationMatrix()						  override;
		virtual glm::mat4 GetRotationMatrix()							  override;
		virtual glm::mat4 GetScaleMatrix()								  override;

		//The Location Returned Is In Screen Coordinates..
		virtual glm::vec2 GetScreenLocation()							  override;
		virtual glm::vec2 GetVelocity()									  override;

		//The Rotation Returned Is In Degrees..
		virtual float GetRotation()										  override;
		virtual float GetAngularVelocity(bool Degrees)					  override;

		//The Scale Returned Is In Times The Original Size :> 
		virtual glm::vec2 GetScale()									  override;

		//Ended Getter Methods..

		virtual void SetPosition(float x, float y)						  override;
		virtual void SetPosition(glm::vec2 NewPos)						  override;
		virtual void Translate(float x, float y)						  override;
		virtual void Translate(glm::vec2 TranslateByHowMuch)			  override;
		virtual void ClearTranslations()								  override;

		virtual void SetVelocity(float x, float y)						  override;
		virtual void SetVelocity(glm::vec2 Velocity)					  override;
		virtual void ClearVelocity()									  override;

		//Just Pass In True If The Angle Is Actually Pi Most Probably It Is In Degrees...(if false is passed then i assume DEGREES).
		virtual void Rotate(float Angle, bool Degrees = true)			  override;
		virtual void SetRotation(float Angle, bool Degrees = true)		  override;
		virtual void ClearRotations()									  override;

		virtual void SetAngularVelocity(float Angle, bool Degrees = true) override;
		virtual void ClearAngularVelocity()								  override;

		virtual void SetScale(float x, float y)							  override;
		virtual void SetScale(glm::vec2 Scale)							  override;
		virtual void Scale(float x, float y)							  override;
		virtual void Scale(glm::vec2 ScaleByHowMuch)					  override;
		virtual void ClearScale()										  override;
#pragma endregion

	public:

	protected:

	protected:

	private:
		//Use This To Get The Actor Updated..
		virtual void OnUpdateTickEventRecieved() override;

	private:
		
	};
}