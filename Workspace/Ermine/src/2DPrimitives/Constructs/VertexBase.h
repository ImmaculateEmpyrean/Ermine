#pragma once

/*
	This Class Is Not In Use As Of Now.. Its Feature Set IS Incomplete Let Alone Implementation
*/

#include<iostream>
#include<vector>
#include<string>

#include<memory>

#include "glm.hpp"

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

	public:

		//Returns The Entire Vertex Data As a Vector Of Floats Has To Be Overloaded In Every Child..
		virtual std::vector<float> GetVertexData();

		//Set Position Coordinates Of The Vertex
		void SetPositonCoordinates(glm::vec3 Position);
		//Get Position Coordinates Of The Vertex
		glm::vec3 GetPositionCoordinates();
		
		//Set VertexColor Values
		void SetVertexColorValue(glm::vec3 Position);
		//Get VertexColor Coordinates Of The Vertex
		glm::vec3 GetVertexColorValue();

		//Get The Number Of Elements In The Vertex..
		virtual int GetNumberOfElementsInVertex();

		//Get The Size Of The Vertex In Bytes..
		virtual int GetVertexSize();

		//This Sets The VertexAttributeArray Associated With This Vertex Must Be Overloaded On Each And every Child..
		virtual void SetVertexAttribArray();

		//Add Two VertexBases Together
		std::vector<float> operator+(const VertexBase& rhs);

	public:

	protected:

	protected:

	private:

	private:
		glm::vec3 Position; //Location 1
		glm::vec3 VertexColor; //Location 2

	};
}