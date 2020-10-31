#pragma once
#include<iostream>
#include<vector>
#include<string>

#include<mutex>

#include<glm.hpp>

#include<Object.h>

#include "EventSystem/EventBroadcastStation.h"
#include "EventSystem/Components/RecieverComponent.h"
#include "EventSystem/EventTypes/OnTickEvent.h"

#include "2DPrimitives/ActorFamily/Actor2D.h"

namespace Ermine
{
	class OrthographicCamera:public Object
	{
	private:
		//You Dont Really Need Two Or More Cameras Right.. So For Now Lets Have it As A Singleton..
		OrthographicCamera();
		~OrthographicCamera();

	public:
		//No Need To Copy Around The Orthographic Camera Since Its A Singleton..
		OrthographicCamera(const OrthographicCamera& rhs) = delete;
		OrthographicCamera operator =(const OrthographicCamera& rhs) = delete;

		//No Need To Move Around The Orthographic Camera Since Its A Singleton..
		OrthographicCamera(OrthographicCamera&& rhs) = delete;
		OrthographicCamera operator =(OrthographicCamera&& rhs) = delete;

		//Use This Function To Get The Camera.. Anytime Anywhere.. Though Its Not Thread Safe Yet..
		static OrthographicCamera* Get(); 

		//This Function Can Only Be Called Once.. After Which The Camera Ceases To Function
		static void Deinitialize();

		//Start These Functions Are Used To See To Get The Data Needed From The Camera..//
		glm::mat4 GetProjectionViewMatrix();
		glm::mat4 GetViewMatrix();
		glm::mat4 GetProjectionMatrix();
		//Ended These Functions Are Used To See To Get The Data Needed From The Camera..//

		glm::vec3 GetCameraLocation();
		void SetCameraLocation(glm::vec2 Point);
		void SetCameraLocation(glm::vec3 Point);

		void TranslateCamera(glm::vec2 TranslateByAmount);
		void TranslateCamera(glm::vec3 TranslateByAmount);
		void SetTranslateVelocityToCamera(glm::vec2 Velocity);
		

		float GetCameraRotation(bool Degrees = true);
		void SetCameraRotation(float RotationInDegrees); //Set Camera Rotation Sire..
		void RotateCameraBy(float Rotation); //Rotate The Camera By These Degrees..

		void SetProjectionMatrix(glm::mat4 ProjectionMatrix); //Set Your Projection Matrix Yourself!!! Why Even Need a Camera Then :>
		void SetFOV(float Left, float Right, float Bottom, float Top); //FOV Of a Orthographic Camera Is More Like A Box Than A Frustrum..
		void SetDepth(float NearLimit, float FarLimit); //This Is Really Not Required In Orthographic Camera Being USed For 2D Scenes.. But Whatever..

		//Center On An Actor
		void CentreOnActor(Actor2DBase* Act,glm::vec2 OffsetOfCameraFromActorCentre,float CentreSpeed = 1.0f,bool InheritRotation = false);
		
		void SetCentreSpeed(float CentreSpeed);
		void SetInheritRotation(bool InheritRotation);

		//Use This Function To Pause Centring On An Actor..
		void StopCentreOnActor();

		//Use This Function To Resume Centring On An Actor..
		void ResumeCentreOnActor();

		//Call This Function Every Frame To Update Camera Properties..
		void OnUpdate();

	public:

	protected:

	protected:

	private:
		//This Function Recalculates ProjectionViewMatrix And ViewMatrix
		void HelperRecalculateViewMatrix();

		//Use This Function To Set The Function To Be Called By The Camera Every Frame..
		void OnTick(std::function<void(float)> OnTickFunction) { this->OnTickFunction = OnTickFunction; }
		
		void OnTickFunctionMessageReciever(Event* Eve);
		void OnTickCameraDefaultProcessing(float DeltaTime);

		std::unique_lock<std::recursive_mutex> GetCameraUniqueLock() { return std::move(std::unique_lock<std::recursive_mutex>(OrthigraphicCameraMutex)); }

	private:
		std::atomic_bool CameraReadyToRecieveEvents = true;
		Ermine::SubscriptionTicket* OnTickEventTicket = nullptr;

		//This Is The Function Which Will Be Run By The Camera As The Event OnTick.. Assign A Function Of Your Choice To Be Run
		std::function<void(float)> OnTickFunction = nullptr;

		static std::once_flag InitializedFlag;
		static OrthographicCamera* Camera;

		Actor2DBase* ActorToCentreOn = nullptr;
		float CentreSpeed = 1.0f;
		bool InheritActorRotation = false;
		glm::vec2 ActorDefaultPosition = glm::vec2(0.0f);
		glm::vec2 OffsetOfCameraFromActorCentre = glm::vec2(0.0f);
		float DefaultRotOfActor = 0.0f; //Value Given In Degrees..

		bool TemperoryPauseCentreOnActor = false;

		//Do Not Mess With Cache..//
		bool FLAG = false;
		bool Initialized = false;
		int Counter = 0;
		int CounterMax= 0;
		//Do Not Mess With Cache..//

		//Start Cache//
		bool CacheUptoDate = false;

		glm::mat4 ProjectionViewMatrix;
		glm::mat4 ViewMatrix;
		glm::mat4 ProjectionMatrix;
		//Ended Cache//

		glm::vec3 CameraPosition;
		glm::vec2 CameraVelocity = glm::vec2(0.0f,0.0f);
		float RotationInDegrees;

		std::recursive_mutex OrthigraphicCameraMutex;
	};
}