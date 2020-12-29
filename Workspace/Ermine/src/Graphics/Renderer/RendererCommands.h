#pragma once
#include<iostream>
#include<vector>
#include<string>

#include "RendererPrimitives/VertexArray.h"
#include "Graphics/Renderer/MaterialSystem/Material.h"

#include "glm.hpp"

namespace Ermine
{
	/*
	* As much as possible this class shall handle all OpenGl calls.. The Renderer Should Not Be Bothered About Making RenderingApi Calls
	*/

	class RendererCommands
	{
	public:
		static void SetClearColor(glm::vec4 ClearColor);

		static void InitializeNewFrame();
		static void FlushFrame();

		//This Function Is Used To Draw Triangles To Screen..
		static void DrawIndexed(std::shared_ptr<Ermine::VertexArray> Vao,std::shared_ptr<Ermine::Material> Mat);

	private:
		static void ImguiNewFrame();
		static void ClearScreen();
	};
}