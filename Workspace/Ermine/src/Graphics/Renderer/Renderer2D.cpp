#include "stdafx.h"
#include "Renderer2D.h"

//This Is an OpenGL Specific Renderer So Not a Big Deal..
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "RendererPrimitives/VertexArray.h"
#include "RendererPrimitives/VertexBuffer.h"
#include "RendererPrimitives/IndexBuffer.h"

#include "Graphics/Renderer/MaterialSystem/Shader.h"
#include "Graphics/Renderer/MaterialSystem/Texture.h"

#include "imgui.h"

#pragma region StaticDeclarationRegion

std::once_flag Ermine::Renderer2D::InitializationFlag;
Ermine::Renderer2D* Ermine::Renderer2D::GlobalRenderer2DObj;

#pragma endregion StaticDeclarationRegion

namespace Ermine
{
	Renderer2D* Renderer2D::Get()
	{
		std::call_once(InitializationFlag, []() {
			GlobalRenderer2DObj = new Renderer2D();
		});

		return GlobalRenderer2DObj;
	}


	void Renderer2D::BeginScene(glm::mat4 Camera, glm::mat4 ProjectionMatrix)
	{
		auto Renderer = Ermine::Renderer2D::Get();
		
		Renderer->CameraMatrix = Camera;
		Renderer->ProjectionMatrix = ProjectionMatrix;

		Renderer->ProjectionViewMatrix = ProjectionMatrix * Camera;

		Renderer->SceneBegin = true;
	}

	void Renderer2D::DrawActor2D(Actor2D* Act)
	{
		auto Renderer = Ermine::Renderer2D::Get();

		Renderer->StowedActors.emplace_back(Act);
	}

	void Renderer2D::EndScene()
	{
		const char* vertexShaderSource = "#version 330 core\n"
			"layout(location = 0) in vec3 aPos;\n"
			"layout(location = 1) in vec3 aVertexColors;\n"
			"layout(location = 2) in vec2 aTexCoord;\n"
			"out vec3 aVertexColor;\n"
			"out vec2 TexCoord;\n"
			"uniform mat4 ModelMatrix;\n"
			"uniform mat4 ProjectionViewMatrix;\n"
			"void main()\n"
			"{\n"
			"TexCoord = vec2(aTexCoord.x, aTexCoord.y);\n"
			"aVertexColor=aVertexColors;\n"
			"gl_Position =ProjectionViewMatrix*ModelMatrix*vec4(aPos, 1.0);\n"//vec4(aPos, 1.0);\n"//"ProjectionViewMatrix*ModelMatrix*vec4(aPos, 1.0);\n"
			"}";

		const char* fragmentShaderSource = "#version 330 core\n"
			"out vec4 FragColor;\n"
			"in vec2 TexCoord;\n"
			"in vec3 aVertexColor;\n"
			"uniform sampler2D texture1;\n"
			"void main()\n"
			"{\n"
			"FragColor = texture(texture1, TexCoord);\n"//vec4(1.0f,1.0f,1.0f,1.0f);\n"//"texture(texture1, TexCoord);\n"
			"}\n";

		auto Renderer = Ermine::Renderer2D::Get();
		assert(Renderer->SceneBegin); //Note Scene Must Have Begun To End Otherwise It cannot Be Ended..
		Renderer->SceneBegin = false;

		static Shader *shd = new Shader(std::string(vertexShaderSource), std::string(fragmentShaderSource));
		shd->Bind();

		VertexArray Vao(Actor2D::GetModelSpaceCoordinates(),Actor2D::GetModelSpaceIndices());
		Vao.Bind();

		std::vector<VertexAttribPointerSpecification> Spec = {
			{3,GL_FLOAT,false},
								  {3,GL_FLOAT,false},
								  {2,GL_FLOAT,false}
		};
		Vao.SetVertexAttribArray(Spec);

		for (auto i : Renderer->StowedActors)
		{
			Vao.Bind();

			glm::mat4 ModelMatrix = i->GetModelMatrix();
			shd->UniformMat4(std::string("ModelMatrix"), ModelMatrix);
			shd->UniformMat4(std::string("ProjectionViewMatrix"), Renderer->ProjectionViewMatrix);

			i->GetSprite()->GetTexture()->Bind(0);
			shd->Uniformi(std::string("texture1"), 0);

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,0);
		}

		Renderer->StowedActors.clear();

	}
}
