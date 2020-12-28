#include "stdafx.h"
#include "Renderer2D.h"

//This Is an OpenGL Specific Renderer So Not a Big Deal..
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "OpenGLErrorChecker.h"

#include "Graphics/Renderer/MaterialSystem/Material.h"
#include "Graphics/Renderer/MaterialSystem/Texture.h"

#include "RendererPrimitives/VertexArray.h"
#include "RendererPrimitives/VertexBuffer.h"
#include "RendererPrimitives/IndexBuffer.h"

namespace Ermine {
	//Renderer Defaults
	static constexpr uint32_t MaxQuads = 25000;
	static constexpr uint32_t MaxVertices = MaxQuads * 4;
	static constexpr uint32_t MaxIndices = MaxQuads * 6;
	static constexpr uint32_t MaxTextureSlots = 32;
	

	std::shared_ptr<Ermine::VertexArray> Vao;
	std::shared_ptr<Ermine::Material>	 TexturedQuadMaterial;

	void Renderer2D::Init()
	{
		Vao = Ermine::VertexArray::Generate();
		TexturedQuadMaterial = Ermine::Material::
	}

}