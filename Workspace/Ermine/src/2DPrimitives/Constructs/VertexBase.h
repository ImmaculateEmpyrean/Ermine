#pragma once

#include<iostream>
#include<vector>
#include<string>

#include "glm.hpp"

#include "Graphics/Renderer/RendererPrimitives/VertexLayout.h"
#include "Graphics/Renderer/RendererPrimitives/VertexDataObject.h"
#include "Graphics/Renderer/RendererPrimitives/Vertex.h"

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
		VertexBase(glm::vec3 PositionData, glm::vec4 VertexColor);

		//All Parameters Are Given In Array Form
		VertexBase(std::vector<float> Vertex);

	public:
		//This Function Is General Purpose It Is Used To Seperate Larger Containers Into Smaller Containers..
		static std::vector<std::vector<float>>&& GetVertexBufferSeperated(std::vector<float>& MultipleVertexBuffers);

		//Returns The Entire Vertex Data As a Vector Of Floats Has To Be Overloaded In Every Child..
		virtual std::vector<VertexDataObject> GetVertexData() const;

		void SetPositionCoordinates(float x, float y)   { this->Position = glm::vec3(x,y,0.0f); }
		void SetPositionCoordinates(glm::vec2 Position) { this->Position = glm::vec3(Position,0.0f); }
		void SetPositionCoordinates(glm::vec3 Position) { this->Position = Position; };
		glm::vec3 GetPositionCoordinates() { return Position; };
		
		void SetVertexColorValue(glm::vec4 VertexColor) { this->VertexColor = VertexColor; };
		glm::vec4 GetVertexColorValue() { return VertexColor; };

		static int GetNumberOfElementsInVertex() { return 7; }
		static int GetVertexSize() { return 7 * sizeof(float); }

		virtual Ermine::VertexLayout GetLayout();

		virtual operator std::vector<float>() const { return std::vector<float>(); } //Temporary..
 		virtual operator std::vector<VertexDataObject>() const;

		//Get Vertex From This Structure..
		virtual Vertex GetVertex();

		//This Function Takes In  A Number As Input And Generates A IndexBuffer Which Draws Quads With That Many Vertices.. See That The Number You Send Is A Multiple Of 4.. ALSO IMP THIS DOES NOT TAKE INTO ACT THE LAST VERTEX IF U SEND 24 0-23 is what matters
		static std::vector<uint32_t>  GenerateIndexBufferQuad(int NumberOfVerticesToTakeIntoAccount = 4); 

		//This Function Is Used To Add Two Vertex Base Togeteher
		//std::vector<float> operator+(VertexBase& rhs) { return GetVertexData() + rhs.GetVertexData(); };
	public:

	protected:

	protected:
		glm::vec3 Position = {0.0f,0.0f,0.0f}; //Location 1
		glm::vec4 VertexColor = { 1.0f,1.0f,1.0f,1.0f }; //Location 2

	private:

	private:
		

	};
}