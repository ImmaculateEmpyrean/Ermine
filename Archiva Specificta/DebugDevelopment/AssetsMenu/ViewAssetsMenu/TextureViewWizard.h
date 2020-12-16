#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<unordered_map>

#include<memory>
#include<algorithm>

#include "Graphics/Renderer/MaterialSystem/Texture.h"

#include "EngineUI/PrecursorWindow.h"
#include "EngineResourceHandlers/GlobalTextureCache.h"

#include "EngineUI/WindowHandler.h"
#include "TextureViewer.h"

namespace Ermine
{
	class TextureViewWizard :public PrecursorWindow
	{
	public:
		TextureViewWizard();
		~TextureViewWizard();

	public:
		TextureViewWizard(const TextureViewWizard& rhs);
		TextureViewWizard operator=(const TextureViewWizard& rhs);

		TextureViewWizard(TextureViewWizard&& rhs);
		TextureViewWizard operator=(TextureViewWizard&& rhs);

		virtual void Draw() override;

	public:

	protected:

	protected:

	private:
		bool HelperShouldIDisplayThisImage(std::shared_ptr<Texture> Tex);
		
		void CopyHelper(const TextureViewWizard& rhs);
		void MoveHelper(TextureViewWizard&& rhs);

	private:
		char* FilterText;

		std::vector<std::shared_ptr<Texture>> Textures;
	};
}