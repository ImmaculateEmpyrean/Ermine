#pragma once
#include<iostream>
#include<vector>
#include<string>

#include "EngineUI/PrecursorWindow.h"

namespace Ermine
{
	class NewTileMap : public PrecursorWindow
	{
	public:
		NewTileMap();
		~NewTileMap(); //Deallocate Buffers Using This Destructor
		
	public:
		virtual void Draw();

	public:

	protected:

	protected:

	private:

	private:
		//Buffer To Store The Name Of The Buffer
		char* NameBuffer;
		//Buffer To Store The Path Of The Buffer
		char* PathBuffer;
	};
}