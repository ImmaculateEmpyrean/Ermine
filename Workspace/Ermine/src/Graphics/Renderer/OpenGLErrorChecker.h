#pragma once
//This Header Contains Code To Fascillitate Error Checking In OpenGL. It Must Be Used With All OpenGL Code..
#include<cassert>

void GLClearError();
bool GLCheckError(const char* function, const char* file, int line);

#if defined(ER_DEBUG_DEVELOP) || defined(ER_DEBUG_SHIP)
#define GLCall(x) GLClearError();\
				  std::cout<<#x<<std::endl;\
				  x;\
				  assert(GLCheckError(#x,__FILE__,__LINE__));
#elif defined(ER_RELEASE_SHIP)
#define GLCall(x) x;

#endif