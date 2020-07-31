#pragma once

namespace Ermine
{
	struct VertexAttribPointerSpecification
	{
		//only specify either 1,2,3,4 
		int NumberOfComponents;
		//Specify Something Like Gl_FLOAT or something like that
		unsigned int TypeOfTheComponent;
		//specify if the component is to be normalized.. DEfault false
		bool Normailized = false;
	};
}