#include "stdafx.h"
#include "Core.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Graphics/Renderer/OpenGLErrorChecker.h"

namespace Ermine
{
	int GetScreenHeight()
	{
		int width = 0, height = 0;
		GLCall(glfwGetWindowSize(glfwGetCurrentContext(), &width, &height));

		return width;
	}

	int GetScreenWidth()
	{
		int width = 0, height = 0;
		GLCall(glfwGetWindowSize(glfwGetCurrentContext(), &width, &height));

		return height;
	}
}