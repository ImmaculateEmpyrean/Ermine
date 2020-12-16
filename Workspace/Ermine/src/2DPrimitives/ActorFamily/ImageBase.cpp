#include "stdafx.h"
#include "ImageBase.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "2DPrimitives/Constructs/Quad.h"
#include "2DPrimitives/Constructs/VertexTextured.h"

namespace Ermine
{
#pragma region Constructors
	ImageBase::ImageBase()
		:
		Actor2DBase(std::make_shared<Ermine::Material>(std::filesystem::path("Shader/Vertex/Actor2DUpdatedWithRenderableTextureModuleVertexShader.vert"),
					std::filesystem::path("Shader/Fragment/Actor2DUpdatedWithRenderableTextureModuleFragmentShader.frag")))
	{
		//Lock Engaged As I Am Interacting With The Internal Memory Of This Class..
		auto Lock = Object::GetObjectMutex();
		ActorSprites.emplace_back(Ermine::Sprite::GenerateSprite("Texture/ErmineNullTexture.png"));
	}

	ImageBase::ImageBase(std::shared_ptr<Sprite> Spr)
		:
		Actor2DBase(std::make_shared<Ermine::Material>(std::filesystem::path("Shader/Vertex/Actor2DUpdatedWithRenderableTextureModuleVertexShader.vert"),
					std::filesystem::path("Shader/Fragment/Actor2DUpdatedWithRenderableTextureModuleFragmentShader.frag")))
	{
		auto Lock = Object::GetObjectMutex();
		ActorSprites.emplace_back(Spr);
	}
	ImageBase::ImageBase(std::vector<std::shared_ptr<Sprite>> SpriteBuffer)
		:
		Actor2DBase(std::make_shared<Ermine::Material>(std::filesystem::path("Shader/Vertex/Actor2DUpdatedWithRenderableTextureModuleVertexShader.vert"),
					std::filesystem::path("Shader/Fragment/Actor2DUpdatedWithRenderableTextureModuleFragmentShader.frag")))
	{
		auto Lock = Object::GetObjectMutex();
		ActorSprites.emplace_back(std::make_shared<SpriteBook>("SpriteBuffer", SpriteBuffer));
		
	}

	ImageBase::~ImageBase()
	{}

	ImageBase::ImageBase(ImageBase& rhs)
		:
		Actor2DBase(rhs)
	{
		auto ForeignLock = rhs.GetObjectMutex();
		auto Lock = Object::GetObjectMutex();

		ActorSprites = rhs.ActorSprites;
	}
	ImageBase& ImageBase::operator=(ImageBase& rhs)
	{
		auto ForeignLock = rhs.GetObjectMutex();
		auto Lock = GetObjectMutex();

		Actor2DBase::operator=(rhs);

		ActorSprites = rhs.ActorSprites;
		return *this;
	}

	ImageBase::ImageBase(ImageBase&& rhs)
		:
		Actor2DBase(std::move(rhs))
	{
		auto ForeignLock = rhs.GetObjectMutex();
		auto Lock = Object::GetObjectMutex();

		ActorSprites = std::move(rhs.ActorSprites);
		
	}
	ImageBase& ImageBase::operator=(ImageBase&& rhs)
	{
		auto ForeignLock = rhs.GetObjectMutex();
		auto Lock = Object::GetObjectMutex();

		Actor2DBase::operator=(std::move(rhs));

		ActorSprites = std::move(rhs.ActorSprites);

		return *this;
	}

	std::vector<Ermine::VertexAttribPointerSpecification> ImageBase::GetVertexArraySpecification()
	{
		return {
					{3,GL_FLOAT,false}, //Vertex Position
					{3,GL_FLOAT,false}, //Vertex Color
					{2,GL_FLOAT,false}, //UV Coordinates
					{1,GL_FLOAT,false}  //Texture Number
		};
	}

	int32_t ImageBase::GetRendererDrawMode()
	{
		return GL_TRIANGLES;
	}

#pragma endregion Constructors

	//Start Setter And Getter For The ActorSprite..// 
	std::shared_ptr<Sprite> ImageBase::GetSprite(int index)
	{
		//Get The Lock To Access The Resources..
		auto Lock = Object::GetObjectMutex();
		return ActorSprites[index];
	}
	std::vector<std::shared_ptr<Ermine::Sprite>> ImageBase::GetSpriteBuffer()
	{
		auto Lock = Object::GetObjectMutex();
		return ActorSprites;
	}
	
	int ImageBase::GetTextureNumber(std::filesystem::path TexturePath)
	{
		int Counter = 0;
		for (auto i : ActorSprites)
		{
			auto Tex = i->GetTexture();
			if (Tex->GetFilePath() == TexturePath)
				return Counter;
			Counter++;
		}

		STDOUTDefaultLog_Warn("Requested The Lookup Of A Texture Which Does Not Exist In The ImageBase.. Hence Returning -1 Which is Probably Illegal..");
		return -1;
	}

	void ImageBase::SetSprite(std::shared_ptr<Sprite> Sprite)
	{
		auto Lock = Object::GetObjectMutex();
		ActorSprites.emplace_back(Sprite);
	}
	void ImageBase::RemoveSprite(int index)
	{
		auto Lock = Object::GetObjectMutex();
		ActorSprites.erase(ActorSprites.begin() + index);
	}


	glm::vec2 ImageBase::GetTopRightUV(int index)
	{
		auto Lock = Object::GetObjectMutex();
		return ActorSprites[index]->GetTopRightUV();
	}
	glm::vec2 ImageBase::GetBottomLeftUV(int index)
	{
		auto Lock = Object::GetObjectMutex();
		return ActorSprites[index]->GetBottomLeftUV();
	}

	std::shared_ptr<Ermine::Sprite> ImageBase::GenSprite(std::filesystem::path TexturePath, glm::vec2 BottomLeft, glm::vec2 TopRight)
	{
		auto Cache = Ermine::GlobalTextureCache::Get();
		std::shared_ptr<Ermine::Texture> Tex = Cache->GetTextureFromFile(TexturePath);

		std::shared_ptr<Ermine::Sprite> Sprite = Ermine::Sprite::GenerateSprite(Tex, BottomLeft, TopRight);

		return Sprite;
	}
}