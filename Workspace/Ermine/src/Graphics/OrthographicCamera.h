#pragma once
#include<iostream>
#include<vector>
#include<string>

#include<mutex>

#include<glm.hpp>

namespace Ermine
{
	class OrthographicCamera
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

		float GetCameraRotation(bool Degrees = true);
		void SetCameraRotation(float RotationInDegrees); //Set Camera Rotation Sire..
		void RotateCameraBy(float Rotation); //Rotate The Camera By These Degrees..

		void SetProjectionMatrix(glm::mat4 ProjectionMatrix); //Set Your Projection Matrix Yourself!!! Why Even Need a Camera Then :>
		void SetFOV(float Left, float Right, float Bottom, float Top); //FOV Of a Orthographic Camera Is More Like A Box Than A Frustrum..
		void SetDepth(float NearLimit, float FarLimit); //This Is Really Not Required In Orthographic Camera Being USed For 2D Scenes.. But Whatever..

	public:

	protected:

	protected:

	private:
		//This Function Recalculates ProjectionViewMatrix And ViewMatrix
		void HelperRecalculateViewMatrix();

	private:
		static std::once_flag InitializedFlag;
		static OrthographicCamera* Camera;

		glm::mat4 ProjectionViewMatrix;
		glm::mat4 ViewMatrix;
		glm::mat4 ProjectionMatrix;

		glm::vec3 CameraPosition;
		float RotationInDegrees;
	};
}