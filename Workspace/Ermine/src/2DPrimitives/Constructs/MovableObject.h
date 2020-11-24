#pragma once
#include<iostream>
#include<vector>
#include<string>

#include<glm.hpp>

#include "Object.h"
#include<mutex>

#include "MutexSystem/Interfaces/IMutex.h"
#include "MutexSystem/MutexGaurd.h"

#include "EventSystem/EventBroadcastStation.h"
#include "EventSystem/Components/RecieverComponent.h"
#include "EventSystem/SubscriptionTicket.h"

namespace Ermine
{
	class Actor2D;

	class MovableObject
	{
	public:
#pragma region Constructors
		//This Can Exist But It Is Not Set To Default..
		MovableObject(); 

		//Send In a Position So That We Can Default To It
		MovableObject(glm::vec2 SpawnPosition);

		//Some Quirky Constructors
		MovableObject(glm::vec2 SpawnPosition, float Rotation);

		//Quirky Constructor 2
		MovableObject(glm::vec2 SpawnPosition, float Rotation, glm::vec2 Scale);

		//Quirky Constructor 3 The Position Is Assumed To Be 0,0 {Top Left Of The Screen Mind You}
		MovableObject(float Rotation, glm::vec2 Scale);

		//Send In a Default Model Matrix So That We Can Default To it..
		MovableObject(glm::mat4 ModelMatrix);

		//Movable Object Must Destroy Data Properly As It Manages Data..
		~MovableObject();

	public:
		//Copy And Move Are To Be Defined Since This Class Manages Memory As Of Now..
		MovableObject(const MovableObject& rhs);
		MovableObject& operator=(const MovableObject& rhs);

		MovableObject(MovableObject&& rhs);
		MovableObject& operator=(MovableObject&& rhs);
		//Start Getter Methods
#pragma endregion

	/*public:
#pragma region IMutexOverrides
		//Start IMutex Overrides//
		virtual std::unique_lock<std::recursive_mutex> GetUniqueLock() override { return std::unique_lock<std::recursive_mutex>(MovableObjectStandradMutex); }
		virtual Ermine::MutexLevel GetMutexLevel() override { return Ermine::MutexLevel::MovableObject; }
		//Ended IMutex Overrides//
#pragma endregion
*/

		//This Is The Most Important Method Of The Class If You Ask Me.. 
		virtual glm::mat4 GetModelMatrix();
		virtual glm::mat4 GetTranslationMatrix();
		virtual glm::mat4 GetRotationMatrix();
		virtual glm::mat4 GetScaleMatrix();

		//The Location Returned Is In Screen Coordinates..
		virtual glm::vec2 GetScreenLocation();
		virtual glm::vec2 GetVelocity();

		//The Rotation Returned Is In Degrees..
		virtual float GetRotation();
		virtual float GetAngularVelocity(bool Degrees);

		//The Scale Returned Is In Times The Original Size :> 
		virtual glm::vec2 GetScale();

		//Ended Getter Methods..

		virtual void SetPosition(float x, float y);
		virtual void SetPosition(glm::vec2 NewPos);
		virtual void Translate(float x, float y);
		virtual void Translate(glm::vec2 TranslateByHowMuch);
		virtual void ClearTranslations();

		virtual void SetVelocity(float x, float y);
		virtual void SetVelocity(glm::vec2 Velocity);
		virtual void ClearVelocity();

		//Just Pass In True If The Angle Is Actually Pi Most Probably It Is In Degrees...(if false is passed then i assume DEGREES).
		virtual void Rotate(float Angle, bool Degrees = true);
		virtual void SetRotation(float Angle, bool Degrees = true);
		virtual void ClearRotations();

		virtual void SetAngularVelocity(float Angle,bool Degrees = true);
		virtual void ClearAngularVelocity();

		virtual void SetScale(float x, float y);
		virtual void SetScale(glm::vec2 Scale);
		virtual void Scale(float x, float y);
		virtual void Scale(glm::vec2 ScaleByHowMuch);
		virtual void ClearScale();

	public:

	protected:

	protected:

	private:
		//Recalculating The Entire Cache Is Slow Compared To Only The Model MAtrix Class That Is Why Two Functions Are Provided.. Try Using Only The HelperRecalculateModelMatrix It Is Bound To Make Our Code Faster
		void HelperRecalculateCache();

		//Use This As Much as Possible..
		void HelperRecalculateModelMatrix();

		//This Function Is Not At All Exposed To The Outside World.. Used By The Movable Object To Update Itself
		void Update();

		void HelperCopy(const MovableObject& rhs);
		void HelperMove(MovableObject&& rhs);

	public:
		//Start Declaration Of The Cache..//
		bool CacheValid = false;
		bool ModelMatrixCacheValid = false;

		glm::mat4 ModelMatrix = glm::mat4(1.0f);

		glm::mat4 TranslationMatrix = glm::mat4(1.0f);
		glm::mat4 RotationMatrix = glm::mat4(1.0f);
		glm::mat4 ScaleMatrix = glm::mat4(1.0f);
		//Ended Declaration Of The Cache..//
		
		//This Position Is Given In Screen Coordinates..
		glm::vec2 Position = glm::vec2(0.0f);

		//Only Rotation In The Z Axis Tracked.. All Other Rotations ARe Simply Ignored..
		float Rotation = 0.0f; 

		//Only Scaling In X and Y Are Tracked.. Scaling In Z Has No Meaning For a 2D Engine Like Ermine..
		glm::vec2 scale = glm::vec2(1.0f, 1.0f);

		//Dependent On Tick
		glm::vec2 Velocity = glm::vec2(0.0f);
		float AngularVelocityInDegrees = 0.0f;

		friend class Ermine::Actor2D;
	};
}