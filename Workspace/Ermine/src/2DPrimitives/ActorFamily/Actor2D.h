#pragma once
#include<iostream>
#include<string>
#include<vector>

#include "ImageBase.h"
#include "2DPrimitives/Constructs/MovableObject.h"
#include "glm.hpp"

#include "Interfaces/IMovableActor.h"

#include "EngineResourceHandlers/GlobalTextureCache.h"

namespace Ermine {

	//An Actor Is Something That Is Displayed And Movable Nothing Else.. Do Not Asume It Contains Only one Sprite Or Any Such Nonesense..
	//This Is The Non Physics Variant Of tHe Actor.. It Is Moved On The screen Without The Help Of Box2D.. For The Aid Of Physics In Moving Something On The Screen Goto PhysicsActor2D..

	//The Actor Class Is Always Described With Respect To Center Of The Quad..
	class Actor2D : public ImageBase ,public MovableObject,public IMovableActor
	{
	public:
		
#pragma region Constructors
		//Having An Actor Without a Sprite At This Point Is Quiet Dangerous..
		Actor2D() = delete;

		//Atleast a sprite is required to construct an actor 
		Actor2D(std::shared_ptr<Sprite> Spr);

		//More Sprites The Better When Constructing The Actor..
		Actor2D(std::vector<std::shared_ptr<Sprite>> SpriteBuffer);

		//This is The Default And Best Constructor
		Actor2D(std::shared_ptr<Sprite> Spr, glm::mat4 ModelMatrix);

		//This is The Default and Best Constructor For SpriteBook
		Actor2D(std::vector<std::shared_ptr<Sprite>> SpriteBuffer, glm::mat4 ModelMatrix);

		//This Is Done For Future Proofing Purpose.. Its Children May One Day Choose To Manage Memory On Their Own..
		virtual ~Actor2D();

		//Start Must Implement Custom Copy And Move As This Class Now Holds A Mutex..//
		Actor2D(Actor2D& rhs);
		Actor2D& operator=(Actor2D& rhs);

		Actor2D(Actor2D&& rhs);
		Actor2D& operator=(Actor2D&& rhs);
		//Ended Must Implement Custom Copy And Move As This Class Now Holds A Mutex..//
#pragma endregion

#pragma region GeneratorFunctions
		static std::shared_ptr<Actor2D> GenerateActor2D(std::filesystem::path TexturePath);
		static std::shared_ptr<Actor2D> GenerateActor2D(std::filesystem::path TexturePath, glm::vec2 ActorScreenLocation, float Rotation = 0.0f, glm::vec2 Scale = {1.0f,1.0f});
		static std::shared_ptr<Actor2D> GenerateActor2D(std::shared_ptr<Sprite> Spr);
		static std::shared_ptr<Actor2D> GenerateActor2D(std::shared_ptr<Sprite> Spr, glm::vec2 ActorScreenLocation, float Rotation = 0.0f, glm::vec2 Scale = { 1.0f,1.0f });
#pragma endregion
		
	public:
		//This Function Has To Be Overriden In all Children Do Not Forget Otherwise One Child May Be Thought Of As The Other..
		virtual Ermine::ActorFamilyIdentifier GetActorFamilyIdentifier()	  override { return ActorFamilyIdentifier::Actor2D; }

		virtual std::vector<float> CalculateModelSpaceVertexes()			  override;

#pragma region IMovableActorOverrides 
		//Start Implementation Of Movable Actor//
		virtual glm::vec2 GetActorPosition()								  override;
		virtual void SetActorPosition(glm::vec2 ActorPosition)				  override;

		virtual glm::vec2 GetActorVelocity()								  override;
		virtual void SetActorVelocity(glm::vec2 ActorVelocity)				  override;

		//virtual float GetAngularVelocity(bool Degrees)						  override;
		//virtual void  SetAngularVelocity(float AngularVelocity, bool Degrees) override;
		//Ended Implementation Of Movable Actor//
#pragma endregion

#pragma region MovableObjectOverrides
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
		static std::shared_ptr<Ermine::Sprite> GenSprite(std::filesystem::path TexturePath,glm::vec2 BottomLeft,glm::vec2 TopRight);
		static glm::mat4 GenModelMatrix(glm::vec2 ActorScreenLocation, float Rotation, glm::vec2 Scale);

		void Actor2DConstructionAssciate();

#pragma region EventProcessing
		//This Function Is Called Every Frame For All Objects Of This Class
		virtual void OnTickEventRecieved(float DeltaTime)				  override;
#pragma endregion
	private:


	};

}