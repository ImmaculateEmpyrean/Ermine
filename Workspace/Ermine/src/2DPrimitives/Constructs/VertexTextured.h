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
		VertexTextured(glm::vec2 VertexUVCoordinates, int TextureNumber);

		//All Parameters Are Given This Constructor
		VertexTextured(glm::vec3 PositionData, glm::vec4 VertexColor, glm::vec2 VertexUVCoordinates, int TextureNumber);

	public:
		//This Function Is General Purpose It Is Used To Seperate Larger Containers Into Smaller Containers..
		static std::vector<std::vector<VertexDataObject>>&& GetVertexBufferSeperated(std::vector<VertexDataObject>& MultipleVertexBuffers);

		//Returns The Entire Vertex Data As a Vector Of Floats Has To Be Overloaded In Every Child..
		virtual std::vector<VertexDataObject> GetVertexData()const override;

		void SetVertexUV(glm::vec2 VertexUVCoordinates) { UV = VertexUVCoordinates; };
		glm::vec2 GetVertexUVCoordinates() { return UV; };

		void SetTextureNumber(int Number) { Tex = Number; };
		int GetTextureNumber() { return Tex; };

		static int GetNumberOfElementsInVertex() { return VertexBase::GetNumberOfElementsInVertex() + 3; };
		static int GetVertexSize() { return VertexBase::GetVertexSize() + (2 * sizeof(float)) + (1 * sizeof(int)); };

		//This Sets The VertexAttributeArray Associated With This Vertex Make Sure You Bound The Required Vao Before Calling This Function Though..
		virtual Ermine::VertexLayout GetVertexBufferLayout() override;

		//This Is Used To Implicitly Convert The Vertex Class Into A Vector Of Floats Anytime AnyPlace.. 
		virtual operator std::vector<Ermine::VertexDataObject>() const override;

		//
	public:

	protected:

	protected:
		glm::vec2 UV = {0.0f,0.0f};
		std::int32_t Tex = 0; //0 is the null texture in renderer as of this writing..

	private:

	private:
		

	};
}