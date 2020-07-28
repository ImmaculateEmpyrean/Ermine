#pragma once

/*
	This Class Is Not In Use As Of Now.. Its Feature Set IS Incomplete Let Alone Implementation
*/

#include<iostream>
#include<vector>
#include<string>

#include<memory>

#include "glm.hpp"
#include "VertexBase.h"

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

	public:

		//Returns The Entire Vertex Data As a Vector Of Floats Has To Be Overloaded In Every Child..
		virtual std::vector<float> GetVertexData();

		//Set VertexUV Values
		void SetVertexUV(glm::vec2 VertexUVCoordinates);
		//Get VertexColor Coordinates Of The Vertex
		glm::vec2 GetVertexUVCoordinates();

		//Set TextureNumber Of The Vertex
		void SetPositonCoordinates(float Position);
		//Get TextureNumber Of The Vertex
		float GetPositionCoordinates();

		//Get The Number Of Elements In The Vertex..
		virtual int GetNumberOfElementsInVertex();

		//Get The Size Of The Vertex In Bytes..
		virtual int GetVertexSize();

		//This Sets The VertexAttributeArray Associated With This Vertex Must Be Overloaded On Each And every Child..
		virtual void SetVertexAttribArray();

		//Add Two VertexBases Together
		std::vector<float> operator+(const VertexTextured& rhs);

	public:

	protected:

	protected:

	private:

	private:
		glm::vec2 VertexUVCoordinates;
		float TextureNumber;

	};
}