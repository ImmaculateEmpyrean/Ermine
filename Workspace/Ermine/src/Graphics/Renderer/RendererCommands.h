#pragma once
#include<iostream>
#include<vector>
#include<string>

//#include "RendererPrimitives/VertexArray.h"
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
		static void DrawIndexed(std::shared_ptr<VertexBuffer> Vbo,std::shared_ptr<IndexBuffer> Ibo,
								std::shared_ptr<Material> Mat);

	private:
		static void ImguiNewFrame();
		static void ClearScreen();
	};
}