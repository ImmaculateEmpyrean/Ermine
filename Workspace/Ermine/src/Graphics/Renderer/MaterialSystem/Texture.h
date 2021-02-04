#pragma once
#include<iostream>
#include<vector>
#include<string>

#include<memory>
#include<filesystem>
#include<fstream>

#include "Core.h"

namespace Ermine
{
	//these classes are defined in the cpp file where we have access to openGL header glad
	enum class textureWrap;
	enum class textureFilter;
	
	struct textureConfig
	{
		textureWrap SAxisWrap = textureWrap::repeat;
		textureWrap TAxisWrap = textureWrap::repeat;

		glm::vec4 borderColor = glm::vec4(1.0f, 0.0f, 0.0f, 0.5f);

		textureFilter minFilter = textureFilter::nearest;
		textureFilter maxFiletr = textureFilter::linear;
	};


	class Texture
	{
	private:
		Texture(std::filesystem::path textureFilePath,textureConfig config);
		~Texture();

	public:
		//texture class never needs to be copied.. it is way too expensive and better pass a handle around..
		Texture(const Texture& rhs) = delete;
		Texture& operator=(const Texture& rhs) = delete;

		//moving the texture class is something also not done regularly but it can be definitely done..
		Texture(Texture&& rhs);
		Texture& operator=(Texture&& rhs);

		static std::shared_ptr<Ermine::Texture> generate(std::filesystem::path texturePath, textureConfig config = textureConfig());

		static std::shared_ptr<Ermine::Texture> getNullTexture();
		std::shared_ptr<Ermine::Texture> getTexture(std::filesystem::path texPath); //this is slower.. as much as possible try to use texture name..

		//call this function to reset texture cache of the factory generate function.. too many calls to this function may make the engine slower but at the same time may reduce overall memory footprint..
		static void factoryReset();

	public:
		//get api
		int getWidth () const;
		int getHeight() const;

		static int getAspectRatioWidth (int Height);
		static int getAspectRatioHeight(int Width );
		int getAspectRatioWidth()  const;
		int getAspectRatioHeight() const;
		
		std::string getName() const;
		std::filesystem::path getFilePath() const;

		operator unsigned int() const;
		unsigned int getID()	const;

		double getAspectRatio() const;

		//set api
		

	public:

	protected:

	protected:

	private:
		void genTexture();
		std::unique_ptr<char> loadTextureData(std::filesystem::path textureDataPath) const;
		void setTextureData(std::unique_ptr<char> data);

	private:
		std::filesystem::path m_textureFilePath; //debug name to be extracted from this if needed and only for debugging purposes..
		
		std::uint32_t m_textureID = 0;

		int m_textureWidth = 0;
		int m_textureHeight = 0;
		int m_NumberOfChannelsInTexture = 0;

		bool m_cacheValid = false;
	};
}