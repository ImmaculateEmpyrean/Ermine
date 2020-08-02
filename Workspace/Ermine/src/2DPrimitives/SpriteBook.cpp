#include "stdafx.h"
#include "SpriteBook.h"

namespace Ermine
{
	SpriteBook::SpriteBook(std::string Name, std::vector<std::shared_ptr<Sprite>> SpriteContainer)
		:
		SpriteBookName(Name),
		SpriteContainer(SpriteContainer),
		Sprite(SpriteContainer[0]->GetTexture(), SpriteContainer[0]->GetBottomLeftUV(), SpriteContainer[0]->GetTopRightUV())
	{}

	/*SpriteBook::SpriteBook(std::filesystem::path JsonFilePath)
	{
		//This Method Is Not Yet Implemented.. It will Be In The Future..
	}*/

	SpriteBook::~SpriteBook()
	{
	}

	void SpriteBook::CalculateFrameCounter()
	{

		if (BottomLeftFeedback || TopRightFeedback)
		{
			BottomLeftFeedback = false;
			TopRightFeedback = false;
			return;
		}
		FrameCounter++;

		if (FrameCounter == SpriteContainer.size())
			FrameCounter = 0;
	}

	std::shared_ptr<Texture> SpriteBook::GetTexture()
	{
		CalculateFrameCounter();
		return SpriteContainer[FrameCounter]->GetTexture();
	}

	glm::vec2 SpriteBook::GetBottomLeftUV()
	{
		BottomLeftFeedback = true;
		return SpriteContainer[FrameCounter]->GetBottomLeftUV();
	}

	glm::vec2 SpriteBook::GetTopRightUV()
	{
		TopRightFeedback = true;
		return SpriteContainer[FrameCounter]->GetTopRightUV();
	}
}
