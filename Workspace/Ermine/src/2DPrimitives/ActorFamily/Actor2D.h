#pragma once
#include<iostream>
#include<string>
#include<vector>

#include "ImageBase.h"
#include "2DPrimitives/Constructs/MovableObject.h"
#include "glm.hpp"

#include "Interfaces/IMovableActor.h"

namespace Ermine {

	//An Actor Is Something That Is Displayed And Movable Nothing Else.. Do Not Asume It Contains Only one Sprite Or Any Such Nonesense..
	//This Is The Non Physics Variant Of tHe Actor.. It Is Moved On The screen Without The Help Of Box2D.. For The Aid Of Physics In Moving Something On The Screen Goto PhysicsActor2D..

	//The Actor Class Is Always Described With Respect To Center Of The Quad..
	class Actor2D : public ImageBase ,public MovableObject, public IMovableActor
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

#pragma region IMutexOverrides
		//Start IMutex Overrides//
		virtual std::unique_lock<std::recursive_mutex> GetUniqueLock() override { return std::move(std::unique_lock<std::recursive_mutex>(Actor2DMutex)); }
		virtual Ermine::MutexLevel GetMutexLevel() override { return Ermine::MutexLevel::Actor2D; }
		virtual Ermine::MutexGaurd GetErmineMutexGaurd() { return std::move(MutexGaurd(this, Ermine::MutexLevel::Actor2D)); };
		//Ended IMutex Overrides//
#pragma endregion
		
	public:

		//This Function Has To Be Overriden In all Children Do Not Forget Otherwise One Child May Be Thought Of As The Other..
		virtual Ermine::ActorFamilyIdentifier GetActorFamilyIdentifier() override { return ActorFamilyIdentifier::Actor2D; }

		virtual std::vector<float> CalculateModelSpaceVertexes() override;

		//This Function Is Overriden So As To Update MovableObject.. But for now it is empty..
		virtual void ClassOnTick(float dt) override {};

#pragma region IMovableActorOverrides 
		//Start Implementation Of Movable Actor//
		virtual glm::vec2 GetActorPosition() override;
		virtual void SetActorPosition(glm::vec2 ActorPosition) override;

		virtual glm::vec2 GetActorVelocity() override;
		virtual void SetActorVelocity(glm::vec2 ActorVelocity) override;

		virtual float GetAngularVelocity(bool Degrees) override;
		virtual void  SetAngularVelocity(float AngularVelocity, bool Degrees) override;
		//Ended Implementation Of Movable Actor//
#pragma endregion

		//This Function Is Used To Get The Centre Of Any Actor On Screen.. 
		virtual glm::vec2 GetScreenLocation() override { return GetScreenLocation(); };
	public:

	protected:

	protected:

	private:
		
	private:
		std::recursive_mutex Actor2DMutex;
	};

}