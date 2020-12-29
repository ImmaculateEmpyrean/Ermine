#include "stdafx.h"
#include "Core.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Graphics/Renderer/OpenGLErrorChecker.h"

int Count = 0;

namespace Ermine
{
	Ermine::DeltaTime Ermine::TimeStep = DeltaTime();

	//This Variable Will Be Set By The Main Function When ExexuteApp Finishes Executing..
	Ermine::App* Ermine::H_App = nullptr;

	Ermine::App* Ermine::GetHandleToApp()
	{ 
		return H_App; 
	}


	int GetScreenHeight()
	{
		int width = 0, height = 0;
		GLCall(glfwGetWindowSize(glfwGetCurrentContext(), &width, &height));

		return height;
	}

	int GetScreenWidth()
	{
		int width = 0,height = 0;
		GLCall(glfwGetWindowSize(glfwGetCurrentContext(), &width, &height));

		return width;
	}


	float NormalizationFunction(float x, float LowestValueInData, float HighestValueInData, float LowestValueInNewRange, float HighestValueInNewRange)
	{
		float y = (((x - LowestValueInData) * (HighestValueInNewRange - LowestValueInNewRange)) / (HighestValueInData - LowestValueInData)) + LowestValueInNewRange;
		return y;
	}

	std::vector<float> operator+(const std::vector<float>& v1, const std::vector<float>& v2)
	{
		std::vector<float> Result;

		for (auto i : v1)
			Result.emplace_back(i);

		for (auto i : v2)
			Result.emplace_back(i);

		return Result;
	}
}
