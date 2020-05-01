#include "stdafx.h"
#include "GlfwKeyCallbacks.h"

//This Is The Reason Not To Include This Header File Anywhere..
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "ErmineKeyCodesDefinition.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	STDOUTDefaultLog_Error("Key Callback Recieved...");
}
