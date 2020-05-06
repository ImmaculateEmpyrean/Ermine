#include "stdafx.h"
#include "OpenGLErrorChecker.h"

//Only For OpenGL..
#include "glad/glad.h"
#include "GLFW/glfw3.h"

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLCheckError(const char* function,const char* file,int line)
{
	bool flag = true;
	while (GLenum error = glGetError())
	{
		STDOUTDefaultLog_Critical("[OpenGL Error] ({}) : {} {} : {}",error,function,file,line);
		flag = false;
	}
	
	return flag;
}
