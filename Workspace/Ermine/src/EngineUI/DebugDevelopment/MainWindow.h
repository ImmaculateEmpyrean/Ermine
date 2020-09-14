#pragma once
#include<iostream>
#include<vector>
#include<string>

#include<filesystem>

#include "../PrecursorWindow.h"

#ifdef ER_DEBUG_DEVELOP

namespace Ermine
{
	class DebugMainWindow:public PrecursorWindow
	{
	public:
		DebugMainWindow();

	public:
		virtual void Draw() override;

	public:

	protected:

	protected:

	private:
		void AddTilesetViewerWindowToWindowHandler(std::filesystem::path TilesetPath);

	private:
		int WindowFlags;

	};
}

#endif