#pragma once

#include<iostream>
#include<vector>
#include<string>

#include "glm.hpp"
#include "VertexBase.h"

#include "Graphics/Renderer/RendererPrimitives/VertexLayout.h"

namespace Ermine
{
	class VertexTextured:public VertexBase
	{
	public:
		//Vertex Can Be Empty There Is Nothing Wrong With It..
		VertexTextured();

		//VertexBase With Only positon Set
		VertexTextured(glm::vec3 PositionData);

		//VertexBase With Both Position And VertexColor Set
		VertexTextured(glm::vec3 PositionData, glm::vec4 VertexColor);

		//VertexBase With Only UV Set
		VertexTextured(glm::vec2 VertexUVCoordinates);

		//VertexBase With Both UV And TextureNumber Set
		VertexTextured(glm::vec2 VertexUVCoordinates, float TextureNumber);

		//All Parameters Are Given This Constructor
		VertexTextured(glm::vec3 PositionData, glm::vec4 VertexColor, glm::vec2 VertexUVCoordinates, float TextureNumber);

		//All Parameters Are Given In Array Form
		VertexTextured(std::vector<float> Vertex);

	public:
		//This Function Is General Purpose It Is Used To Seperate Larger Containers Into Smaller Containers..
		static std::vector<std::vector<float>>&& GetVertexBufferSeperated(std::vector<float>& MultipleVertexBuffers);

		//Returns The Entire Vertex Data As a Vector Of Floats Has To Be Overloaded In Every Child..
		virtual std::vector<float> GetVertexData() const override;

		void SetVertexUV(glm::vec2 VertexUVCoordinates) { UV = VertexUVCoordinates; };
		glm::vec2 GetVertexUVCoordinates() { return UV; };

		void SetTextureNumber(float Number) { Tex = Number; };
		float GetTextureNumber() { return Tex; };

		static int GetNumberOfElementsInVertex() { return VertexBase::GetNumberOfElementsInVertex() + 3; };
		static int GetVertexSize() { return VertexBase::GetVertexSize() + (3 * sizeof(float)); };

		//This Sets The VertexAttributeArray Associated With This Vertex Make Sure You Bound The Required Vao Before Calling This Function Though..
		virtual Ermine::VertexLayout GetVertexBufferLayout() override;

		//This Is Used To Implicitly Convert The Vertex Class Into A Vector Of Floats Anytime AnyPlace.. 
		virtual operator std::vector<float>() const override { return GetVertexData(); }

	public:

	protected:

	protected:
		glm::vec2 UV = {0.0f,0.0f};
		float Tex = 33.0f;

	private:

	private:
		

	};
}