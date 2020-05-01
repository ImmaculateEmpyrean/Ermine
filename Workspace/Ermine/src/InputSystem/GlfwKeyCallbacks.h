#pragma once
#include<iostream>
#include<vector>
#include<string>

#include "EventSystem/Components/BroadcastComponent.h"

struct GLFWwindow; //Forward Declaration So That Succeeding Functions Donot Complain..

/*
	According To The Doccumentation Both key_callback and character_callback are not equal. They Simply Appear To Be
	"Keys and characters do not map 1:1. A single key press may produce several characters, and a single character may require several 
	 keys to produce. This may not be the case on your machine, but your users are likely not all using the same keyboard layout, 
	 input method or even operating system as you." - GLFW Doccumentation Page
*/
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void character_callback(GLFWwindow* window, unsigned int codepoint);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

//Did Not Implement Clipboard Functionality from Glfw Doccumentation See Bottom Almost Near The End For Callback For Clipboard Text... 
//https://www.glfw.org/docs/3.3/input_guide.html