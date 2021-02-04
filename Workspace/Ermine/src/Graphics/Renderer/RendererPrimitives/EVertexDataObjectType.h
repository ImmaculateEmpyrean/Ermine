#pragma once
#include<iostream>
#include<vector>
#include<string>

//TODO- Get rid of this header as this certainly not needed here
#include<glad/glad.h>
#include<GLFW/glfw3.h>

namespace Ermine
{
	enum class EVertexDataObjectType : unsigned int
	{
		FLOAT = GL_FLOAT,

		INT  = GL_INT,
		UINT = GL_UNSIGNED_INT,

		BYTE  = GL_BYTE,
		UBYTE = GL_UNSIGNED_BYTE,
	};
}