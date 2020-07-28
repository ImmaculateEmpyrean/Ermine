#pragma once

/*
	This Module Will Be Inherited By The Sprite Class Giving It Scope To Upload The Texture Contained..
*/

#include<iostream>
#include<vector>
#include<string>

#include<memory>

#include "Texture.h"
#include "Material.h"

namespace Ermine
{
	class RenderableTextureModule
	{
	public:
		//An Empty Renderable Texture Module Can Exist Though It is Suboptimal In Nature And Should Be Avoided..
		RenderableTextureModule();

		//There Is No Point In This Module Existing Without A Texture Existing Please See That As Much As Possible This Constructor Is Called..
		RenderableTextureModule(std::shared_ptr<Texture> Tex);

	public:

	public:

	protected:

	protected:

	protected:

	private:

	private:
		std::shared_ptr<Texture> TextureContained;
	};
}