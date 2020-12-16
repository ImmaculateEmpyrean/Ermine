#include "stdafx.h"
#include "Texture.h"

#include "stb_image.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Graphics/Renderer/OpenGLErrorChecker.h"

#include "EngineResourceHandlers/GlobalTextureCache.h"

namespace Ermine
{
	Texture::Texture()
	{
		Name = "NoName";
		HelperGenTexture();
	}
	Texture::Texture(std::filesystem::path TextureFilePath)
		:
		TextureFilePath(TextureFilePath),
		Name(TextureFilePath.u8string())
	{
		HelperGenTexture();

		if (!TextureFilePath.empty())
			HelperLoadDataWithStbiAndCallglTexImage2D();
		
	}
	Texture::Texture(std::filesystem::path TextureFilePath, std::string Name)
		:
		TextureFilePath(TextureFilePath),
		Name(Name)
	{
		HelperGenTexture();

		if (!TextureFilePath.empty())
			HelperLoadDataWithStbiAndCallglTexImage2D();
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &Tex);
	}


	Texture::Texture(const Texture& rhs)
		:
		TextureFilePath(rhs.TextureFilePath),
		Name(rhs.Name)
	{
		HelperGenTexture();

		if (!TextureFilePath.empty())
		{
			HelperLoadDataWithStbiAndCallglTexImage2D();
		}
	}
	Texture Texture::operator=(const Texture& rhs)
	{
		TextureFilePath = rhs.TextureFilePath;
		Name = rhs.Name;
		HelperGenTexture();

		if (!TextureFilePath.empty())
		{
			HelperLoadDataWithStbiAndCallglTexImage2D();
		}

		return *this;
	}

	Texture::Texture(Texture&& rhs)
	{
		TextureFilePath = rhs.TextureFilePath;
		Name = rhs.Name;
		Tex = rhs.Tex;

		width = rhs.width;
		height = rhs.height;
		NumberOfChannels = rhs.NumberOfChannels;

		rhs.Tex = 0;
	}
	Texture Texture::operator=(Texture&& rhs)
	{
		TextureFilePath = rhs.TextureFilePath;
		Name = rhs.Name;
		Tex = rhs.Tex;

		width = rhs.width;
		height = rhs.height;
		NumberOfChannels = rhs.NumberOfChannels;

		rhs.Tex = 0;

		return *this;
	}

	std::shared_ptr<Ermine::Texture> Texture::GetNullTexture()
	{
		auto TextureManager = Ermine::GlobalTextureCache::Get();
		auto Tex = TextureManager->GetTextureFromFile("Texture/ErmineNullTexture.png");
		return Tex;
	}

	void Texture::Bind(unsigned int slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot); // activate the texture unit first before binding texture
		glBindTexture(GL_TEXTURE_2D, Tex);
	}

	void Texture::Unbind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	int Texture::GetWidth()
	{
		return width;
	}

	int Texture::GetHeight()
	{
		return height;
	}

	int Texture::GetWidthToMatchAspectRatio(int Height)
	{
		float AspectRatio = (float)(this->width) / (float)(this->height);
		return Height * AspectRatio;
	}

	int Texture::GetHeightToMatchAspectRatio(int Width)
	{
		float AspectRatio = (float)(this->height) / (float)(this->width);
		return Width * AspectRatio;
	}

	std::string Texture::GetName()
	{
		return Name;
	}

	std::filesystem::path Texture::GetFilePath()
	{
#if defined(ER_DEBUG_DEVELOP) || defined(ER_DEBUG_SHIP)
		if (TextureFilePath.empty())
		{
			STDOUTDefaultLog_Error("Error Texture Filepath Empty For The Texture Name : {}", Name);
		}
#endif
		return TextureFilePath;
	}

	Texture::operator unsigned int()
	{
		return Tex;
	}
	unsigned int Texture::GetTextureID()
	{
		return Tex;
	}

	double Texture::GetAspectRatio()
	{
		return width / height;
	}


	void Texture::HelperGenTexture()
	{
		GLCall(glGenTextures(1, &Tex));
		GLCall(glBindTexture(GL_TEXTURE_2D, Tex));

		// set the texture wrapping/filtering options (on the currently bound texture object)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// load and generate the texture
	}
	void Texture::HelperLoadDataWithStbiAndCallglTexImage2D()
	{
		stbi_set_flip_vertically_on_load(1);
		unsigned char* TextureData = stbi_load(TextureFilePath.u8string().c_str(), &width, &height, &NumberOfChannels, 4);
		if (TextureData)
		{
			GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, TextureData));
			GLCall(glGenerateMipmap(GL_TEXTURE_2D));
		}
		else
		{
			STDOUTDefaultLog_Error("Error STB Image Failed To Load Desire Texture : {}", TextureFilePath.u8string().c_str());
		}
		stbi_image_free(TextureData);
	}
}