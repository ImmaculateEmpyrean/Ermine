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
		Texture(std::filesystem::path TextureFilePath,std::string Name="NoName");

		~Texture();

	public:
		Texture(const Texture& rhs);
		Texture operator=(const Texture& rhs);

		Texture(Texture&& rhs);
		Texture operator=(Texture&& rhs);

		void Bind(int slot=0);
		void Unbind();

		int GetWidth();
		int GetHeight();

		std::string GetName();

		operator unsigned int();
		unsigned int GetTextureID();

		double GetAspectRatio();

	public:

	protected:

	protected:

	private:
		void HelperGenTexture();
		void HelperLoadDataWithStbiAndCallglTexImage2D();

	private:
		std::filesystem::path TextureFilePath;
		std::string Name;

		unsigned int Tex;

		int width=0;
		int height=0;
		int NumberOfChannels=0;

	};
}