#pragma once
#include<iostream>
#include<vector>
#include<string>

#include "glm.hpp"

#include "RenderableTextureModule.h"

namespace Ermine
{
	class RenderablePhysicsComponent2D : public RenderableTextureModule
	{
	public:
		//There Is No Point in Constructing Such A Specialized Object For No good Reason Better Delete The Empty Constructor For It Only Complicates Matters..
		RenderablePhysicsComponent2D() = delete;

		//This Is The Easiest Way To Construct A RenderablePhysicsComponent2D..
		RenderablePhysicsComponent2D(RenderableTextureModule Obj, glm::mat4 TranslationMatrix);

		//A Virtual Destructor May Be Useful In The Context Of Children..
		virtual ~RenderablePhysicsComponent2D();

	public:
		//Get The Translation Matrix Stored Inside This Object..
		glm::mat4 GetTranslationMatrix();
		//Set The Translation Matrix Stored Inside This Object..
		void SetTranslationMatrix(glm::mat4 TranslationMatrix);

	public:

	protected:

	protected:

	private:

	private:
		//This Matrix Must Be Passed Into the GPU.. For The Model To Be Translated..
		glm::mat4 TranslationMatrix;

	};
}