#include "stdafx.h"
#include "Core.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Graphics/Renderer/OpenGLErrorChecker.h"

int Count = 0;

namespace Ermine
{
	Ermine::DeltaTime Ermine::TimeStep = DeltaTime();

	void PrintCount()
	{
		std::cout <<"Count : "<< Count << std::endl;
	}
	int GetCount()
	{
		return Count;
	}

	void ResetCount()
	{
		Count = 0;
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
}
