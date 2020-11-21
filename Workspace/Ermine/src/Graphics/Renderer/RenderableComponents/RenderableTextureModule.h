#pragma once

/*
	This Module Will NOT Be Inherited By The Sprite Class
*/

#include<iostream>
#include<vector>
#include<string>

#include<memory>
#include<filesystem>

#include "Graphics/Renderer/MaterialSystem/Texture.h"
#include "Graphics/Renderer/MaterialSystem/Material.h"

#include "Renderable2D.h"

#include "EngineResourceHandlers/GlobalTextureCache.h"

namespace Ermine
{
	class RenderableTextureModule: public Ermine::Renderable2D
	{
	protected:
		//This Is The Only Constructor As Of Now And Is Used By The Generator Exclusively..
		RenderableTextureModule(std::shared_ptr<Ermine::Actor2D> Ptr, std::vector<std::shared_ptr<Ermine::Texture>> Textures);

		RenderableTextureModule(RenderableTextureModule& rhs);
		RenderableTextureModule& operator=(RenderableTextureModule& rhs);

		RenderableTextureModule(RenderableTextureModule&& rhs);
		RenderableTextureModule& operator=(RenderableTextureModule&& rhs);

#pragma region Renderable2DOverrides
		virtual void Initialize() override;
		virtual void Refresh()	  override;
#pragma endregion


#pragma region Generator
		static std::unique_ptr<Ermine::RenderableTextureModule> Generate(std::shared_ptr<Actor2DBase> Act);
#pragma endregion


	public:
		//The Texture Path Is Submitted To The GlobalTextureCache From which a Texture Is Recieved For The Buffer
		virtual void SubmitTexture(std::filesystem::path TexturePath);
		virtual void SubmitTexture(std::shared_ptr<Texture> Texture);

		//Binds Textures Contained Inside The Renderable Texture Module And Returns An Array Containing In which Slot Each Texture Is Bound..
		virtual std::vector<int> BindTexturesContained();

		//Clears Out The Contents Of The RenderableTextureModuleBuffer And Its PARENTS TOO...
		virtual void Clear() override;
		virtual void ClearTextureBuffer();

	public:

	protected:

	protected:

	protected:

	private:

	private:
		//Hold All The Textures.. The Primary Job Of this module..
		std::vector<std::shared_ptr<Texture>> TexturesBuffer;

	};
}