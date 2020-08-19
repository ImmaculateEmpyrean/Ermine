#pragma once

/*
	This Class Is Not In Use As Of Now.. Its Feature Set IS Incomplete Let Alone Implementation
*/

#include<iostream>
#include<vector>
#include<string>

#include<memory>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include"Graphics/Renderer/RendererPrimitives/VertexArray.h"

#include "glm.hpp"
#include "VertexBase.h"

//Start Overload Operator + For std::vector<float>
//This Operator Is Overloaded And Setup So That Vertex Classes Are Easily Added If And When Encountered..
std::vector<float> operator+(const std::vector<float>& v1, const std::vector<float>& v2);
//End Overload Operator + For std::vector<float>

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
		VertexTextured(glm::vec3 PositionData, glm::vec3 VertexColor);

		//VertexBase With Only UV Set
		VertexTextured(glm::vec2 VertexUVCoordinates);

		//VertexBase With Both UV And TextureNumber Set
		VertexTextured(glm::vec2 VertexUVCoordinates, float TextureNumber);

		//All Parameters Are Given This Constructor
		VertexTextured(glm::vec3 PositionData, glm::vec3 VertexColor, glm::vec2 VertexUVCoordinates, float TextureNumber);

		//All Parameters Are Given In Array Form
		VertexTextured(std::vector<float> Vertex);

	public:
		//This Function Is General Purpose It Is Used To Seperate Larger Containers Into Smaller Containers..
		virtual std::vector<std::vector<float>>&& GetVertexBufferSeperated(std::vector<float>& LargeContainerHavingMyltipleVertexBuffers );

		//Returns The Entire Vertex Data As a Vector Of Floats Has To Be Overloaded In Every Child..
		virtual std::vector<float> GetVertexData() const override;

		//Set VertexUV Values
		void SetVertexUV(glm::vec2 VertexUVCoordinates);
		//Get VertexColor Coordinates Of The Vertex
		glm::vec2 GetVertexUVCoordinates();

		//Set TextureNumber Of The Vertex
		void SetTextureNumber(float TextureNumber);
		//Get TextureNumber Of The Vertex
		float GetTextureNumber();

		//Get The Number Of Elements In The Vertex..
		virtual int GetNumberOfElementsInVertex();

		//Get The Size Of The Vertex In Bytes..
		virtual int GetVertexSize();

		//This Sets The VertexAttributeArray Associated With This Vertex Make Sure You Bound The Required Vao Before Calling This Function Though..
		virtual void SetVertexAttribArray(VertexArray& Vao);

		//This Is Used To Implicitly Convert The Vertex Class Into A Vector Of Floats Anytime AnyPlace.. 
		virtual operator std::vector<float>() const override { return GetVertexData(); }

	public:

	protected:

	protected:
		glm::vec2 VertexUVCoordinates = {-92.0f,-92.0f };
		float TextureNumber = -93.0f;

	private:

	private:
		

	};
}