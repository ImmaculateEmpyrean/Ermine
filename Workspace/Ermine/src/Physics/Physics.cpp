#include "stdafx.h"
#include "Physics.h"

#include<iostream>
#include<vector>
#include<string>

//The Variable Is Defined in this translation unit as it could not be defined in a header file..
b2World* Ermine::Universum = nullptr;

//This Variable Is Used To Convert To And From The Pixel Space..
float Ermine::ScaleFactor = 0.01f;

glm::vec2 Ermine::coordWorldToPixels(glm::vec2 world)
{
	return glm::vec2();
}
glm::vec2 Ermine::coordWorldToPixels(float worldX, float worldY)
{
	return glm::vec2();
}
