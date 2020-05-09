#pragma once
#include<iostream>
#include<vector>
#include<string>

#include "Graphics/Renderer/MaterialSystem/Texture.h"

#include "EngineUI/PrecursorWindow.h"
#include "EngineResourceHandlers/GlobalTextureCache.h"

namespace Ermine
{
	class TextureViewWizard :public PrecursorWindow
	{
	public:
		TextureViewWizard();
		~TextureViewWizard();

	public:
		virtual void Draw() override;

	public:

	protected:

	protected:

	private:

	private:
		char* FilterText;

		std::vector<Texture*> Textures;
	};
}