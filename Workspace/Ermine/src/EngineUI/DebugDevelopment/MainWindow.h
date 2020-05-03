#pragma once
#include<iostream>
#include<vector>
#include<string>

#include "../PrecursorWindow.h"

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

	private:
		int WindowFlags;

	};
}