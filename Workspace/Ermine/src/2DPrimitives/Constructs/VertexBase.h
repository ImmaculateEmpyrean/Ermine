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

#include "Graphics/Renderer/RendererPrimitives/VertexAttribPointerSpecification.h"

namespace Ermine
{
	class VertexBase
	{
	public:
		//Vertex Can Be Empty There Is Nothing Wrong With It..
		VertexBase();

		//VertexBase With Only positon Set
		VertexBase(glm::vec3 PositionData);

		//VertexBase With Both Position And VertexColor Set
		VertexBase(glm::vec3 PositionData, glm::vec3 VertexColor);

		//All Parameters Are Given In Array Form
		VertexBase(std::vector<float> Vertex);

	public:

		//Returns The Entire Vertex Data As a Vector Of Floats Has To Be Overloaded In Every Child..
		virtual std::vector<float> GetVertexData() const;

		//Set Position Coordinates Of The Vertex
		void SetPositonCoordinates(glm::vec3 Position);
		//Get Position Coordinates Of The Vertex
		glm::vec3 GetPositionCoordinates();
		
		//Set VertexColor Values
		void SetVertexColorValue(glm::vec3 VertexColor);
		//Get VertexColor Coordinates Of The Vertex
		glm::vec3 GetVertexColorValue();

		//Get The Number Of Elements In The Vertex..
		virtual int GetNumberOfElementsInVertex();

		//Get The Size Of The Vertex In Bytes..
		virtual int GetVertexSize();

		//This Sets The VertexAttributeArray Associated With This Vertex Must Be Overloaded On Each And every Child..
		virtual void SetVertexAttribArray(VertexArray& Vao);

		virtual operator std::vector<float>() const { return GetVertexData(); }

		//This Function Takes In  A Number As Input And Generates A IndexBuffer Which Draws Quads With That Many Vertices.. See That The Number You Send Is A Multiple Of 4.. ALSO IMP THIS DOES NOT TAKE INTO ACT THE LAST VERTEX IF U SEND 24 0-23 is what matters
		static std::vector<uint32_t>  GenerateIndexBufferQuad(int NumberOfVerticesToTakeIntoAccount = 4); 

	public:

	protected:

	protected:
		glm::vec3 Position = {-90.0f,-90.0f,-90.0f}; //Location 1
		glm::vec3 VertexColor = { -91.0f,-91.0f,-90.1f }; //Location 2

	private:

	private:
		

	};
}