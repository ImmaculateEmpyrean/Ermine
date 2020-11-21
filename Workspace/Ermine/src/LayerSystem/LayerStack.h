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

#include "Object.h"

namespace Ermine
{
	//Forward Declaration For Future Convienience..
	class Renderer2D;

	class LayerStack : Ermine::Object
	{
	public:
		LayerStack(std::string Name);
		~LayerStack();

	public:
		LayerStack(LayerStack& rhs) = delete;
		LayerStack& operator=(LayerStack& rhs) = delete;

		LayerStack(LayerStack&& rhs);
		LayerStack& operator=(LayerStack&& rhs);

#pragma PushLayerOntoStack
		void PushLayerOntoStackFront(std::unique_ptr<Ermine::LayerStackLayer> LayerToPush);
		void PushLayerOnTheBackOfTheStack(std::unique_ptr<Ermine::LayerStackLayer> LayerToPush);
		void PushLayerOntoStackAtPosition(std::unique_ptr<Ermine::LayerStackLayer> LayerToPush, int index);

		void PushLayerOntoStackFront(std::string LayerName);
		void PushLayerOnTheBackOfTheStack(std::string LayerName);
		void PushLayerOntoStackAtPosition(std::string LayerName, int index);

		//Return The Number Of Layers Held By The Layer Stack Structure
		int GetLayerStackSize();
#pragma endregion

		//If Two Or More Layers USe The Same Name.. Returns The First Thing It Encounters.. It Is Upto The Developer To Enforce Proper Names.. Ermine Does Not Care..
		std::unique_ptr<Ermine::LayerStackLayer> GetLayer(std::string LayerName);
		//This Is Returns The Layer With The Stack Number.. See That You Donot Go Over Size..
		std::unique_ptr<Ermine::LayerStackLayer> GetLayer(int IndexInTheStack);
		
		//Clears All The Layers Of This LayerStack..
		void Clear();

		//No Changes Will Be Made To The TileMap As There is No Need To Make Any Kind Of Changes We Simply Get The Necessary Renderable And Be Done Wit It..
		//void SubmitTileMapForDrawing(Ermine::TileMap const * Tm); Not Supported For Now..
	public:

	protected:

	protected:

	private:
		//This Function Is In Charge Of Recieving Events For Processing..
		void RecieveEvents(Ermine::Event* Eve);

	private:
		std::string LayerStackName; //This More Akin To A Debug Name And Not To Be Taken Seriously...
		std::vector<std::unique_ptr<Ermine::LayerStackLayer>> LayersBuffer;

		friend class Renderer2D;
	};
}