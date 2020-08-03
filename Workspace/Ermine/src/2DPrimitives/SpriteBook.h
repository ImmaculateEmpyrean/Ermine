#pragma once

/*
	Note - Delta Time Is Not Yet Implemented In Ermine At The Time Of Writing It Will Be Coming In The Future..
	The Api Will Be Fleshed Out once Support For Delta Timer Is Added Into Ermine..
*/

#include<iostream>
#include<memory>
#include<vector>
#include<string>

#include<fstream>
#include<filesystem>
#include "nlohmann/json.hpp"

#include "Sprite.h"

namespace Ermine
{
	class SpriteBook : public Sprite
	{
	public:
		//This Is The Most Rudimentary Container This Is Done To Primarily Initialize Temporary SpriteBooks..
		SpriteBook(std::string Name, std::vector<std::shared_ptr<Sprite>> SpriteContainer);

		//This Is Much More Of A Sophisticated Constructor.. You Need To Generate a Json Before You Can Actually Use This Constructor
		//SpriteBook(std::filesystem::path JsonFilePath);

		virtual ~SpriteBook() override;

	public:
		//Start Implementing Overriden Contents From Sprite.//
		virtual std::shared_ptr<Texture> GetTexture() override;

		virtual glm::vec2 GetBottomLeftUV() override;
		virtual glm::vec2 GetTopRightUV() override;
		//Ended Implementing Overriden Contents From Sprite.//

		int GetAnimationFps() { return AnimationFps; }
		void SetAnimationFps(int Fps) { AnimationFps = Fps; }

		std::string GetSpriteBookName() { return SpriteBookName; }
		void SetSpriteBookName(std::string NewName) { SpriteBookName = NewName; }

	public:

	protected:

	protected:

	private:
		//This Advances The Frame Counter If Set In Delta Time Mode This May Behave Slightly Differently..
		void CalculateFrameCounter();

	private:
		//Animation Frame Rate
		int AnimationFps = 10;

		//The Name Is Generally Used Only For Debug Purposes..
		std::string SpriteBookName;

		//The Container To Hold Sprites..
		std::vector<std::shared_ptr<Sprite>> SpriteContainer;

		//Current Frame We Are In..
		double FrameCounter = 0;
		
		//Feedback
		bool BottomLeftFeedback = false;
		bool TopRightFeedback = false;

	};
}