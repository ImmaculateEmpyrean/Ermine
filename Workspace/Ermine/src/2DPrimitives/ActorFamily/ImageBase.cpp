#include "stdafx.h"
#include "ImageBase.h"

#include "2DPrimitives/Constructs/Quad.h"
#include "2DPrimitives/Constructs/VertexTextured.h"

namespace Ermine
{
#pragma region Constructors
	ImageBase::ImageBase(std::shared_ptr<Sprite> Spr)
		:
		Actor2DBase(), //Invoking The Default Constructor I Guess..
		Actorsprite(Spr)
	{
		auto Lock = Object::GetObjectMutex();

		//Setup The Renderable to Be Working..
		RefreshRenderable2D(); 
	}
	ImageBase::ImageBase(std::vector<std::shared_ptr<Sprite>> SpriteBuffer)
		:
		Actor2DBase() //Invoking The Default Constructor I Guess..
	{
		auto Lock = Object::GetObjectMutex();
		
		//A SpriteBuffer Properly Translates To A SpriteBook..
		//A SpriteBook Has The Same Interface As The Sprite So I dont See Much Of A Problem..
		Actorsprite = std::make_shared<SpriteBook>("SpriteBuffer", SpriteBuffer);

		//Setup The Renderable To be Working..
		RefreshRenderable2D();
	}

	ImageBase::~ImageBase()
	{}

	ImageBase::ImageBase(ImageBase& rhs)
		:
		Actor2DBase(rhs),
		RenderableTextureModule(rhs)
	{
		auto ForeignLock = rhs.GetObjectMutex();
		auto Lock = Object::GetObjectMutex();

		Actorsprite = rhs.Actorsprite;
	}
	ImageBase& ImageBase::operator=(ImageBase& rhs)
	{
		auto ForeignLock = rhs.GetObjectMutex();
		auto Lock = Object::GetObjectMutex();

		Actor2DBase::operator=(rhs);
		RenderableTextureModule::operator=(rhs);

		Actorsprite = rhs.Actorsprite;
		return *this;
	}

	ImageBase::ImageBase(ImageBase&& rhs)
		:
		Actor2DBase(std::move(rhs)),
		RenderableTextureModule(std::move(rhs))
	{
		auto ForeignLock = rhs.GetObjectMutex();
		auto Lock = Object::GetObjectMutex();

		Actorsprite = std::move(rhs.Actorsprite);
		rhs.Actorsprite = nullptr;
	}
	ImageBase& ImageBase::operator=(ImageBase&& rhs)
	{
		auto ForeignLOck = rhs.GetObjectMutex();
		auto Lock = Object::GetObjectMutex();

		Actor2DBase::operator=(std::move(rhs));
		RenderableTextureModule::operator=(std::move(rhs));

		Actorsprite = std::move(rhs.Actorsprite);
		return *this;
	}

#pragma endregion Constructors

	//Start Setter And Getter For The ActorSprite..// 
	std::shared_ptr<Sprite> ImageBase::GetSprite()
	{
		//Get The Lock To Access The Resources..
		auto Lock = Object::GetObjectMutex();
		return Actorsprite;
	}
	void ImageBase::SetSprite(std::shared_ptr<Sprite> Sprite)
	{
		auto Lock = Object::GetObjectMutex();

		Actorsprite = Sprite;
		RenderableTextureModule::Clear();
		RenderableTextureModule::SubmitTexture(Actorsprite->GetTexture());
	}
	//Ended Setter And Getter For The ActorSprite..//


	//The Quad Class Already Contains A Nice Static Function.. Just Gotta Make Use Of It..
	std::vector<uint32_t> ImageBase::GetIndices()
	{
		return Ermine::Quad::GetModelIndices();
	}
	std::vector<float> ImageBase::CalculateModelSpaceVertexes()
	{
		Ermine::VertexTextured TopRight(Quad::GetModelCoordinatesTopRight());
		Ermine::VertexTextured BottomRight(Quad::GetModelCoordinatesBottomRight());
		Ermine::VertexTextured BottomLeft(Quad::GetModelCoordinatesBottomLeft());
		Ermine::VertexTextured TopLeft(Quad::GetModelCoordinatesTopLeft());

		glm::vec3 TopRightPos = TopRight.GetPositionCoordinates();
		glm::vec4 TopRightPos4 = glm::vec4(TopRightPos, 0.0f);

		glm::vec3 BottomRightPos = BottomRight.GetPositionCoordinates();
		glm::vec4 BottomRightPos4 = glm::vec4(BottomRightPos, 0.0f);

		glm::vec3 BottomLeftPos = BottomLeft.GetPositionCoordinates();
		glm::vec4 BottomLeftPos4 = glm::vec4(BottomLeftPos, 0.0f);

		glm::vec3 TopLeftPos = TopLeft.GetPositionCoordinates();
		glm::vec4 TopLeftPos4 = glm::vec4(TopLeftPos, 0.0f);

		TopRight.SetPositonCoordinates(TopRightPos4);
		BottomRight.SetPositonCoordinates(BottomRightPos4);
		BottomLeft.SetPositonCoordinates(BottomLeftPos4);
		TopLeft.SetPositonCoordinates(TopLeftPos4);

		//Get The Mutex As We Are Starting To Use Shared Memory
		auto Lock = Object::GetObjectMutex();

		TopRight.SetVertexUV(glm::vec2(Actorsprite->GetTopRightUV().x, Actorsprite->GetBottomLeftUV().y));
		BottomRight.SetVertexUV(glm::vec2(Actorsprite->GetTopRightUV().x, Actorsprite->GetTopRightUV().y));
		BottomLeft.SetVertexUV(glm::vec2(Actorsprite->GetBottomLeftUV().x, Actorsprite->GetTopRightUV().y));
		TopLeft.SetVertexUV(glm::vec2(Actorsprite->GetBottomLeftUV().x, Actorsprite->GetBottomLeftUV().y));

		//We Are Done Using Shared Resources..
		Lock.unlock();

		std::vector<float> ModelCoordinates;
		ModelCoordinates = TopRight;
		ModelCoordinates = ModelCoordinates + BottomRight;
		ModelCoordinates = ModelCoordinates + BottomLeft;
		ModelCoordinates = ModelCoordinates + TopLeft;


		return ModelCoordinates;
	}
	

	//Calculate Model Vertexes And Update The Renderable U Know The One thing That Actually Mattres When Drawing
	void ImageBase::RefreshRenderable2D()
	{
		auto Lock = Object::GetObjectMutex();

		auto Vao = Ermine::VertexArray(VertexBuffer(CalculateModelSpaceVertexes()), 
									   IndexBuffer(GetIndices()));
		
		auto Spec = GetVertexAttribSpecificationForTheActor();
		Vao.SetVertexAttribArray(Spec);

		Renderable2D::SetVertexArray(std::move(Vao));
		Renderable2D::SetMaterial(Ermine::Material(std::filesystem::path("Shader/Vertex/Actor2DUpdatedWithRenderableTextureModuleVertexShader.vert"),
			std::filesystem::path("Shader/Fragment/Actor2DUpdatedWithRenderableTextureModuleFragmentShader.frag")));
		RenderableTextureModule::SubmitTexture(Actorsprite->GetTexture());
	}


	std::vector<VertexAttribPointerSpecification> ImageBase::GetVertexAttribSpecificationForTheActor()
	{
		static std::vector<VertexAttribPointerSpecification> Spec = {
				{3,GL_FLOAT,false},
				{3,GL_FLOAT,false},
				{2,GL_FLOAT,false},
				{1,GL_FLOAT,false}
		};
		return Spec;
	}

	std::vector<int> ImageBase::BindTexturesContained()
	{
		std::vector<int> BoundVector;
		BoundVector.resize(16, 0);

		auto TextureCacheGlobal = Ermine::GlobalTextureCache::Get();

		//Acquire The Lock Here As We Are Trying To Use Shared Resources..
		auto Lock = Object::GetObjectMutex();

		int BoundSlot = TextureCacheGlobal->Bind(Actorsprite->GetTexture());
		BoundVector[0] = BoundSlot;

		//This Totally Must Not Be Here.. And Also Consider Creating a New COmponent Which Allows U to Set Uv's At Will..
		//It Is Not expected we Literally Refresh The Renderable(aka generate a new one) inside the bind function.. it will end up too expensive than common perception 
		RefreshRenderable2D();

		return BoundVector;
	}


#pragma region RenderableTextureModuleExposition
	void ImageBase::SubmitTexture(std::filesystem::path TexturePath)
	{
		//Accquire The Lock As We Are About To Use Shared Memory
		auto Lock = GetObjectMutex();

		RenderableTextureModule::SubmitTexture(TexturePath);
	}
	void Ermine::ImageBase::SubmitTexture(std::shared_ptr<Texture> Texture)
	{
		//Accquire The Lock As We Are About To Use Shared Memory
		auto Lock = GetObjectMutex();

		RenderableTextureModule::SubmitTexture(Texture);
	}

	/*std::vector<std::shared_ptr<Texture>>& Ermine::ImageBase::GetBuffer()
	{
	//This Function Cannot Be Implemented In Current Archietecture	
	}*/


	void Ermine::ImageBase::Clear()
	{
		//Accquire The Lock As We Are About To Use Shared Memory
		auto Lock = GetObjectMutex();

		RenderableTextureModule::Clear();
	}
	void Ermine::ImageBase::ClearTextureBuffer()
	{
		//Accquire The Lock As We Are About To Use Shared Memory
		auto Lock = GetObjectMutex();

		RenderableTextureModule::ClearTextureBuffer();
	}
#pragma endregion

}