#pragma once
#include<iostream>
#include<vector>
#include<string>

#include<memory>
#include<filesystem>
#include<fstream>

namespace Ermine
{
	class Texture
	{
	public:
		//Empty Texture Can Exist But Would Need Some Setup
		Texture();

		//More Like Default Constructor Of Texture
		Texture(std::filesystem::path TextureFilePath);

		~Texture();

	public:
		Texture(const Texture& rhs);
		Texture operator=(const Texture& rhs);

		Texture(Texture&& rhs);
		Texture operator=(Texture&& rhs);

		void Bind(int slot=0);
		void Unbind();
	public:

	protected:

	protected:

	private:
		void HelperGenTexture();
		void HelperLoadDataWithStbiAndCallglTexImage2D();

	private:
		std::filesystem::path TextureFilePath;

		unsigned int Tex;

		int width=0;
		int height=0;
		int NumberOfChannels=0;

	};
}