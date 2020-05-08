#pragma once
#include<iostream>
#include<vector>
#include<string>

#include<filesystem>
#include<fstream>

#include "EngineUI/PrecursorWindow.h"

namespace Ermine
{
	class AddNewTextureWindow : public PrecursorWindow
	{
	public:
		AddNewTextureWindow();
		~AddNewTextureWindow();

	public:
		virtual void Draw();
	public:

	protected:

	protected:

	private:

	private:
		const char* BufferToStoreFilePath;
		bool ShowErrorPathIncorrectWindow = false;
	};
}