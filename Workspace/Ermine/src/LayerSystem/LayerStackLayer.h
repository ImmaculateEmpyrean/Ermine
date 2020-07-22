#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<memory>

#include "EventSystem/Event.h"

#include "2DPrimitives/Renderable2D.h"
#include "2DPrimitives/Actor2D.h"

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

		//Clear All Submitted Renderables
		void Clear();

	public:

	protected:
		virtual bool HandleConcreteEventInLayerAndPassForward(Ermine::Event* EventPointer);

	protected:

	private:
		void HelperCopyConstructor(const LayerStackLayer& rhs);
		void HelperMoveConstructor(LayerStackLayer&& rhs);

		void HelperEmplaceRenderableInRenderablesContainer(Renderable2D* RenderableObj);

		bool HandleEventInLayerAndPassForward(Ermine::Event* EventPointer);
		
	private:
		std::string LayerName;
		std::vector<Renderable2D*> Renderables;

		friend class Ermine::Renderer2D;
		friend class Ermine::LayerStack;
	};
}