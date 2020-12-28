#pragma once
#include<iostream>
#include<vector>
#include<string>

#include "glm.hpp"

namespace Ermine
{
	/*
	* As much as possible this class shall handle all OpenGl calls.. The Renderer Should Not Be Bothered About Making RenderingApi Calls
	*/

	class RendererCommands
	{
		static void SetClearColor(glm::vec4 ClearColor);
		static void ClearScreen();

		static void ImguiNewFrame();
		static void InitializeNewFrame();
		static void FlushFrame();

		//This Function Is Used To Draw Triangles To Screen..
		static void DrawIndexed(std::size_t SizeOfIndexBuffer);
	};
}