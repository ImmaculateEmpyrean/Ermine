#pragma once
#include<iostream>
#include<vector>
#include<string>

#include "glm.hpp"

#include "ImageBase.h"
#include "Interfaces/IMovableActor.h"

#include "2DPrimitives/Constructs/MovableObject.h"
#include "2DPrimitives/Constructs/VertexTextured.h"

#include "Graphics/Renderer/MaterialSystem/Texture.h"

#include "Graphics/Renderer/RenderableComponents/RenderableShapeComponent.h"

namespace Ermine
{
	class Actor2DShape: public MovableObject, public ImageBase,public RenderableShapeComponent, public IMovableActor
	{
	public:
#pragma region Constructors
		//Well You Can Totally Have An Empty Shape.. Just Not An Default One..
		Actor2DShape();

		//This Is A Slightly Better Constructor As It Gives You Control Over Where The Image Is Drawn
		Actor2DShape(glm::vec2 SpawnPosition);

		//Probably Use This More Often..
		Actor2DShape(std::vector<Ermine::VertexTextured> Points);

		//Guess This Is The Best For Now..
		Actor2DShape(glm::vec2 SpawnPosition,std::vector<Ermine::VertexTextured> Points);

		//Start Copy And Move Constructor Are Custom Implemented.. For Now Just Because..
		Actor2DShape(Actor2DShape& rhs);
		Actor2DShape& operator=(Actor2DShape& rhs);

		Actor2DShape(Actor2DShape&& rhs);
		Actor2DShape& operator=(Actor2DShape&& rhs);
		//Ended Copy And Move Constructor Are Custom Implemented.. For Now Just Because..

		//The Destructor Has To Be Implemented Probably
		virtual ~Actor2DShape();
#pragma endregion

	public:
		unsigned int GetNumberOfPointsHeld();
		Ermine::VertexTextured& GetPoint(unsigned int Index);

		void AddPoint(Ermine::VertexTextured Point);
		void DeletePoint(unsigned int Index);

#pragma region Actor2DBaseImplementation
		virtual glm::vec2 GetScreenLocation() { return GetActorPosition(); };
#pragma endregion

#pragma region IMovableActorImplementation
		virtual glm::vec2 GetActorPosition() override;
		virtual void SetActorPosition(glm::vec2 ActorPosition)override;

		virtual glm::vec2 GetActorVelocity() override;
		virtual void SetActorVelocity(glm::vec2 ActorVelocity) override;

		virtual float GetAngularVelocity(bool Degrees) override;
		virtual void  SetAngularVelocity(float AngularVelocity, bool Degrees) override;	 
#pragma endregion
	public:

	protected:

	protected:

	private:
		

	private:
		std::vector<Ermine::VertexTextured> Vertexes;
	};
}