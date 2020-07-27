#pragma once

/*
	A Label Is Used To Primarily Draw Text U Submit a Label To a Layer Which When Parsed By The Renderer Draws Text..
*/

#include<iostream>
#include<vector>
#include<string>

#include<algorithm>
#include<memory>

#include "glm.hpp"
#include "2DPrimitives/Renderable2D.h"

#include "freetype-gl-master/freetype-gl.h"

namespace Ermine
{
	class Label : public Renderable2D
	{
	public:
		//An Empty Label Makes No Sense Though It Could Exist I Suppose
		Label() = default; 
		Label(std::string Text, glm::vec3 Color);

	public:

	public:

	protected:

	protected:

	private:

	private:


	};
}