#include "stdafx.h"
#include "WindowHandler.h"

Ermine::WindowHandler* Ermine::WindowHandler::GlobalWindowHandler = nullptr;

void Ermine::WindowHandler::SubmitWindowFront(std::unique_ptr<PrecursorWindow> WindowToBeSubmitted)
{
	WindowsBuffer.emplace_back(std::move(WindowToBeSubmitted));
}

void Ermine::WindowHandler::SubmitWindowBack(std::unique_ptr<PrecursorWindow> WindowToBeSubmitted)
{
	WindowsBuffer.insert(WindowsBuffer.begin(), std::move(WindowToBeSubmitted));
}

std::unique_ptr<Ermine::PrecursorWindow> Ermine::WindowHandler::PopWindowFromStack()
{
	std::unique_ptr<Ermine::PrecursorWindow> WinPtr = std::move(WindowsBuffer[WindowsBuffer.size() - 1]);
	WindowsBuffer.pop_back();
	return WinPtr;
}

void Ermine::WindowHandler::UpdateDraw()
{
	for (int i=0;i<WindowsBuffer.size();i++)
	{
		if (!WindowsBuffer[i]->ShouldIQuit())
		{
			WindowsBuffer[i]->Draw();
		}
		else
		{
			WindowsBuffer.erase(WindowsBuffer.begin() + i);
		}
	}
}