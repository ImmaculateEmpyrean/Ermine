#pragma once
						//note - Layers Stacks Are Parsed As Of Now From Top To Bottom Top Being The Last Element

#include<iostream>
#include<vector>
#include<string>
#include<atomic>
#include<optional>

#include "LayerSystem/LayerStackLayer.h"

#include "EventSystem/EnumEventType.h"
#include "EventSystem/EventBroadcastStation.h"
#include "EventSystem/Event.h"

#include "EventSystem/Components/RecieverComponent.h"
#include "EventSystem/Components/BroadcastComponent.h"

#include "2DPrimitives/TileMap.h"
#include "Graphics/Renderer/RendererPrimitives/RendererPrimitives2D/TileMapRendererPrimitive.h"

namespace Ermine
{
	//Forward Declaration For Future Convienience..
	class Renderer2D;

	class LayerStack
	{
	public:
		LayerStack(std::string Name);
		~LayerStack();

	public:
		LayerStack(const LayerStack& rhs);
		LayerStack operator=(const LayerStack& rhs);

		LayerStack(LayerStack&& rhs);
		LayerStack operator=(LayerStack&& rhs);

		//Each LayerStackLayer Must Be Part Of Exactly One Layer Stack As The Stack Will Destroy Layer When It Sees Fit..
		void PushLayerOntoStackFront(std::unique_ptr<Ermine::LayerStackLayer> LayerToPush);

		//Each LayerStackLayer Must Be Part Of Exactly One Layer Stack As The Stack Will Destroy Layer When It Sees Fit..
		void PushLayerOntoStackAtPosition(std::unique_ptr<Ermine::LayerStackLayer> LayerToPush,int index);

		//Each LayerStackLayer Must Be Part Of Exactly One Layer Stack As The Stack Will Destroy Layer When It Sees Fit..
		void PushLayerOnTheBackOfTheStack(std::unique_ptr<Ermine::LayerStackLayer> LayerToPush);

		//Using The Index Get The Specified Layer From The LayerStack
		Ermine::LayerStackLayer* GetLayer(int IndexInTheStack) { return AllLayersAssociated[IndexInTheStack]; }

		//This Method Must Never BE Used In Distribution Builds It Is Only For Debug Builds
#if defined(ER_DEBUG_DEVELOP) || defined(ER_DEBUG_SHIP)
		std::optional<Ermine::LayerStackLayer*> GetIndexFromName(std::string LayerStackLayerName);
#endif

		void Clear();

		//No Changes Will Be Made To The TileMap As There is No Need To Make Any Kind Of Changes We Simply Get The Necessary Renderable And Be Done Wit It..
		//void SubmitTileMapForDrawing(Ermine::TileMap const * Tm);

	public:

	protected:

	protected:

	private:
		//This is The Main Recieving Event It Will Recieve All Events From The Station First...
		void RecievedEvent(Ermine::Event* EventPointer);

		virtual void RecievedEventConcreteEvent(Ermine::Event* EventPointer); //There Is No Implicit Requirement To recieve this event right

		void HelperCopyConstructor(const LayerStack& rhs);
		void HelperMoveConstructor(LayerStack&& rhs);

	private:
		std::string LayerStackName; //This More Akin To A Debug Name And Not To Be Taken Seriously...
		std::vector<Ermine::LayerStackLayer*> AllLayersAssociated;

		//flags
		std::atomic<bool> RecieveConcreteEvents = true;

		
		friend class Renderer2D;
	};
}