#pragma once
#include<iostream>
#include<vector>
#include<string>

#include "EngineUI/PrecursorWindow.h"

#include "Graphics/Renderer/MaterialSystem/Texture.h"

//This Class Is Used To View And Make Modifications To a Single Texture..

namespace Ermine
{
	class TextureViewer : public PrecursorWindow
	{
	public:
		TextureViewer(std::shared_ptr<Texture> Tex); //This Is The Texture Which Will Be Viewed By The User..

	public:
		virtual void Draw() override;

	public:

	protected:

	protected:

	private:

	private:
		std::shared_ptr<Texture> Tex; //This Is The Texture To Be Displayed
		std::string WindowName;
	};
}