#include "stdafx.h"
#include "RenderablePhysicsComponent2D.h"

namespace Ermine
{
#pragma region Constructors
	
	RenderablePhysicsComponent2D::RenderablePhysicsComponent2D(RenderableTextureModule Obj, glm::mat4 TranslationMatrix)
		:
		RenderableTextureModule(std::move(Obj)),
		TranslationMatrix(TranslationMatrix)
	{}
	

	RenderablePhysicsComponent2D::~RenderablePhysicsComponent2D()
	{
		//This Is Not Used As Of Now.. As This Class Manages No Memory On Its Own..
	}
#pragma endregion Constructors

#pragma region GettersAndSetters
	
	glm::mat4 RenderablePhysicsComponent2D::GetTranslationMatrix()
	{
		return TranslationMatrix;
	}
	void RenderablePhysicsComponent2D::SetTranslationMatrix(glm::mat4 TranslationMatrix)
	{
		this->TranslationMatrix = TranslationMatrix;
	}

#pragma endregion GettersAndSetters
}
