#pragma once
#include<iostream>
#include<vector>
#include<string>

#include<mutex>

#include "glm.hpp"
#include "2DPrimitives/Actor2D.h"

namespace Ermine
{
	class Renderer2D
	{
	public:
		Renderer2D() = default;
		~Renderer2D(); //Renderer2D Destructor..

	public:
		static Renderer2D* Get();

		static void BeginScene(glm::mat4 CameraMAtrix,glm::mat4 ProjectionMatrix);

		static void DrawActor2D(Actor2D* Act);

		static void EndScene();

		void ClearStowedActors();

	public:

	protected:

	protected:

	private:

	private:
		static std::once_flag InitializationFlag;
		static Renderer2D* GlobalRenderer2DObj;

		glm::mat4 CameraMatrix;
		glm::mat4 ProjectionMatrix;
		glm::mat4 ProjectionViewMatrix;

		bool SceneBegin;

		std::vector<Actor2D*> StowedActors;
	};
}