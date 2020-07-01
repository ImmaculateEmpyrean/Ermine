#include "stdafx.h"
#include "Core.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Graphics/Renderer/OpenGLErrorChecker.h"

int Count = 0;

namespace Ermine
{
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
}



/*void* operator new(size_t size)
{
	Count++;
	std::cout << "New operator allocating : " << size<<std::endl;
	void* p = malloc(size);
	return p;
}

void operator delete(void* p)
{
	Count--;
	std::cout << "Delete operator Deleting" <<std::endl;
	free(p);
}*/