#pragma once
#include<iostream>
#include<vector>
#include<string>

struct GLFWwindow; //Forward Declaration So That Succeeding Functions Donot Complain..

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);