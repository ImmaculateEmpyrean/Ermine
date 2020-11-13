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
	public:
		//An Empty Renderable Texture Module Can Exist Though It is Suboptimal In Nature And Should Be Avoided..
		RenderableTextureModule();

		//There Is No Point In This Module Existing Without A Texture Existing Please See That As Much As Possible This Constructor Is Called..
		RenderableTextureModule(std::vector<std::shared_ptr<Texture>> Tex);

		//This Constructor Aims To Setup Renderable2D Without Touching The TextureModule..
		RenderableTextureModule(VertexArray Vao);

		//This Constructor Aims To Setup Renderable2D Without Touching The TextureModule..
		RenderableTextureModule(VertexArray Vao,Material Mat);

		//This Constructor Aims To Setup Renderable2D Partially While Setting Up The TextureModule..
		RenderableTextureModule(VertexArray Vao, std::vector<std::shared_ptr<Texture>> Tex);

		//This Constructor Aims To Setup Renderable2D Partially While Setting Up The TextureModule..
		RenderableTextureModule(Material Mat, std::vector<std::shared_ptr<Texture>> Tex);

		//This Constructor Is All Inclusive Setup Both Rendererable2D and TextureModule.. 
		RenderableTextureModule(VertexArray Vao, Material Mat, std::vector<std::shared_ptr<Texture>> Tex);

		//Consider Making This Virtual.. Access For All These Functions Will Be Given BY Image Base.. Which Can Lock The Mutex Properly..
	public:
		//The Texture Path Is Submitted To The GlobalTextureCache From which a Texture Is Recieved For The Buffer
		virtual void SubmitTexture(std::filesystem::path TexturePath);

		//Submit A Texture To Be Held By The Renderable Texture Module..
		virtual void SubmitTexture(std::shared_ptr<Texture> Texture);

		//A Handle To the Textures Buffer To Modify It In Some Sense.. Note Renderable Texture Module Does Not Have A Delete Function..
		//virtual std::vector<std::shared_ptr<Texture>>& GetBuffer();

		//Binds Textures Contained Inside The Renderable Texture Module And Returns An Array Containing In which Slot Each Texture Is Bound..
		virtual std::vector<int> BindTexturesContained();

		//Clears Out The Contents Of The RenderableTextureModuleBuffer And Its PARENTS TOO...
		virtual void Clear();

		//Clear Out Only The Texture Buffer Of The RenderableTextureModule..
		virtual void ClearTextureBuffer();

	public:

	protected:

	protected:

	protected:

	private:

	private:
		std::vector<std::shared_ptr<Texture>> TexturesBuffer;
	};
}