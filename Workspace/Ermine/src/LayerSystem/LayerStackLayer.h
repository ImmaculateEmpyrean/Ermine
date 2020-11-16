#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<memory>

#include "EventSystem/Event.h"

#include "Graphics/Renderer/RenderableComponents/Renderable2D.h"
#include "2DPrimitives/ActorFamily/Actor2D.h"

#include "2DPrimitives/ActorFamily/PhysicsActor2D.h"
#include "Graphics/Renderer/RenderableComponents/RenderablePhysicsComponent2D.h"

#include "2DPrimitives/TileMapLayerRenderable.h"
#include "2DPrimitives/Label.h"

#include "Physics/Physics.h"

namespace Ermine
{
	//Forward Declaration For The Purposes Of Being a friend...
	class Renderer2D;
	class LayerStack;

	class LayerStackLayer
	{
	public:
		LayerStackLayer(std::string Name);
		~LayerStackLayer();

	public:
		LayerStackLayer(const LayerStackLayer& rhs);
		LayerStackLayer operator=(const LayerStackLayer& rhs);

		LayerStackLayer(LayerStackLayer&& rhs);
		LayerStackLayer operator=(LayerStackLayer&& rhs);

		//Note This Function Will Cause The Layer To Copy Said Object Into Memory.. 
		void SubmitRenderable(Renderable2D* RenderableObj);

		void AddLabel(std::string Text, glm::vec3 Color,glm::vec3 PositionInScreenCoordinates,std::string FontName);

		//Clear All Submitted Renderables
		void Clear();

	public:

	protected:
		virtual bool HandleConcreteEventInLayerAndPassForward(Ermine::Event* EventPointer);

	protected:

	private:
		void HelperCopyConstructor(const LayerStackLayer& rhs);
		void HelperMoveConstructor(LayerStackLayer&& rhs);

		//void HelperEmplaceRenderableInRenderablesContainer(std::shared_ptr<Renderable2D> RenderableObj);
		void HelperEmplaceRenderableInRenderablesContainer(Renderable2D* RenderableObj);

		bool HandleEventInLayerAndPassForward(Ermine::Event* EventPointer);
		
	private:
		std::string LayerName;
		std::vector<std::shared_ptr<Ermine::Renderable2D>> Renderables;

		friend class Ermine::Renderer2D;
		friend class Ermine::LayerStack;
	};
}