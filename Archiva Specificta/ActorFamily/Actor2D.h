#pragma once
#include<iostream>
#include<string>
#include<vector>

#include "glm.hpp"

#include "Actor2DShape.h"

namespace Ermine {

	//An Actor Is Something That Is Displayed And Movable Nothing Else.. Do Not Asume It Contains Only one Sprite Or Any Such Nonesense..
	//This Is The Non Physics Variant Of tHe Actor.. It Is Moved On The screen Without The Help Of Box2D.. For The Aid Of Physics In Moving Something On The Screen Goto PhysicsActor2D..

	//The Actor Class Is Always Described With Respect To Center Of The Quad..
	class Actor2D : public Actor2DShape
	{
	protected:
		
#pragma region Constructors
		//Having An Actor Without a Sprite At This Point Is Quiet Dangerous..
		Actor2D();

		//This is The Default and Best Constructor For SpriteBook
		Actor2D(std::vector<std::shared_ptr<Sprite>> SpriteBuffer, glm::mat4 ModelMatrix);

	public:
		//This Is Done For Future Proofing Purpose.. Its Children May One Day Choose To Manage Memory On Their Own..
		virtual ~Actor2D();

		//Start Must Implement Custom Copy And Move As This Class Needs To Copy And Move Actor2DShape..//
		Actor2D(Actor2D& rhs);
		Actor2D& operator=(Actor2D& rhs);

		Actor2D(Actor2D&& rhs);
		Actor2D& operator=(Actor2D&& rhs);
		//Start Must Implement Custom Copy And Move As This Class Needs To Copy And Move Actor2DShape..//
#pragma endregion

#pragma region GeneratorFunctions
		//Generate Functions For Actor..
		static std::shared_ptr<Actor2D> GenerateActor2D(std::filesystem::path TexturePath);
		static std::shared_ptr<Actor2D> GenerateActor2D(std::filesystem::path TexturePath, glm::vec2 ActorScreenLocation, float Rotation = 0.0f, glm::vec2 Scale = {1.0f,1.0f});
		static std::shared_ptr<Actor2D> GenerateActor2D(std::shared_ptr<Sprite> Spr);
		static std::shared_ptr<Actor2D> GenerateActor2D(std::shared_ptr<Sprite> Spr, glm::vec2 ActorScreenLocation, float Rotation = 0.0f, glm::vec2 Scale = { 1.0f,1.0f });
#pragma endregion
		
	public:
		virtual Ermine::ActorFamilyIdentifier GetActorFamilyIdentifier()	  override { return ActorFamilyIdentifier::Actor2D; }

#pragma region RenderableGenerationImperatives
		virtual std::vector<float> GenerateModelSpaceVertexBuffer() override;
		virtual std::vector<uint32_t> GenerateModelSpaceIndices() override;
		virtual int32_t GetRendererDrawMode() override;
#pragma endregion
	public:

	protected:

	protected:

	private:
		static glm::mat4 GenModelMatrix(glm::vec2 ActorScreenLocation, float Rotation, glm::vec2 Scale);

	private:


	};

}