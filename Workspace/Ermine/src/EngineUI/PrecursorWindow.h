#pragma once
#include<iostream>
#include<vector>
#include<string>

namespace Ermine
{
	class PrecursorWindow
	{
	public:
		PrecursorWindow() = default; //Really No Reason To Have Anything Else

	public:
		//This Actually Has To Be Implemented To Draw The Window...
		virtual void Draw() = 0; 
		
		//This Returns The Quit Flag..
		bool ShouldIQuit() { return Quit; }

	public:
		bool Quit = false;

	protected:

	protected:

	public:

	public:


	};
}