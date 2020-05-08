#include "stdafx.h"
#include "GlfwKeyCallbacks.h"

//This Is The Reason Not To Include This Header File Anywhere..
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "imgui.h"

#include "ErmineKeyCodesDefinition.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	STDOUTDefaultLog_Error("Key Callback Recieved...");
	Ermine::BroadcastComponent::BroadcastEvent(std::make_unique<Ermine::KeyCallbackEvent>(key, scancode, action, mods));
}

void character_callback(GLFWwindow* window, unsigned int codepoint)
{
	STDOUTDefaultLog_Error("{}",char(codepoint));
	Ermine::BroadcastComponent::BroadcastEvent(std::make_unique<Ermine::CharacterCallbackEvent>(codepoint));
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	STDOUTDefaultLog_Error("X:{} , Y:{}", xpos,ypos);
	Ermine::BroadcastComponent::BroadcastEvent(std::make_unique<Ermine::CursorPositionCallbackEvent>((int)xpos,(int)ypos));
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == ERMINE_MOUSE_BUTTON_RIGHT && action == ERMINE_PRESS)
	{
		STDOUTDefaultLog_Error("Right Mouse Button Pressed !!!!!!!");
	}
	Ermine::BroadcastComponent::BroadcastEvent(std::make_unique<Ermine::MouseButtonCallbackEvent>(button,action,mods));
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	STDOUTDefaultLog_Error("X Offset:{} , Y Offset:{}", xoffset, yoffset);
	Ermine::BroadcastComponent::BroadcastEvent(std::make_unique<Ermine::ScrollCallbackEvent>(xoffset,yoffset));
}