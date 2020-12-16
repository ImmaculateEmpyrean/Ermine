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
		Texture(std::filesystem::path TextureFilePath,std::string Name);

		~Texture();

	public:
		Texture(const Texture& rhs);
		Texture operator=(const Texture& rhs);

		Texture(Texture&& rhs);
		Texture operator=(Texture&& rhs);

		//Returns Ermine Null Texture.. Dunno Maybe Use It For Debugging..
		static std::shared_ptr<Ermine::Texture> GetNullTexture();

	public:
		void Bind(unsigned int slot=0);
		void Unbind();

		int GetWidth();
		int GetHeight();

		int GetWidthToMatchAspectRatio(int Height);
		int GetHeightToMatchAspectRatio(int Width);

		std::string GetName();
		std::filesystem::path GetFilePath();

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