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
		virtual void Draw() override;

		AddNewTextureWindow(const AddNewTextureWindow& rhs);
		AddNewTextureWindow operator=(const AddNewTextureWindow& rhs);

		AddNewTextureWindow(AddNewTextureWindow&& rhs);
		AddNewTextureWindow operator=(AddNewTextureWindow&& rhs);

	public:

	protected:

	protected:

	private:
		void CopyHelper(const AddNewTextureWindow& rhs);
		void MoveHelper(AddNewTextureWindow&& rhs);

	private:
		char* BufferToStoreFilePath;
		char* BufferToStoreName;

		bool ShowErrorPathIncorrectWindow = false;
	};
}