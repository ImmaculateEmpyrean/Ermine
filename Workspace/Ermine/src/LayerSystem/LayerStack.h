#pragma once
						//note - First Element In The Stack With Index Zero Is The Top Most Element..
#include<iostream>
#include<vector>
#include<string>

#include "Object.h"

#include "LayerSystem/LayerStackLayer.h"

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
		//These Layers Are All Unique Pointers As The Layers Do Not Have A Mutex In Them.. Soo If Two Different Layer Stacks On Two Threads May Crash By Concurrent Writing Or May Put The Program in a race condition.. 
		//Specifying No Index Will Push The Layer As The BottomMost Layer..
		void PushLayer(std::unique_ptr<Ermine::LayerStackLayer> LayerToPush,int index = 0);
		
		//No Index Means The TopMost layer is Deleted..
		void DeleteLayer(int index = 0);
		void DeleteLayer(std::string LayerName);

		//Return The Number Of Layers Held By The Layer Stack Structure
		int GetSize();
#pragma endregion
		//If Two Or More Layers Use The Same Name.. Returns The First Thing It Encounters.. It Is Upto The Developer To Enforce Proper Names.. Ermine Does Not Care..
		std::unique_ptr<Ermine::LayerStackLayer> GetLayer(std::string LayerName);
		//This Is Returns The Layer With The Stack Number.. See That You Donot Go Over Size..
		std::unique_ptr<Ermine::LayerStackLayer> GetLayer(int IndexInTheStack);
		
		//Clears All The Layers Of This LayerStack..
		void Clear();

	public:

	protected:

	protected:

	private:
		//This Function Is In Charge Of Recieving Events For Processing..
		void RecieveEvents(Ermine::Event* Eve);

	private:
		std::string LayerStackName = "NoNameLayerStack";

		//Holds The Layers..
		std::vector<std::unique_ptr<Ermine::LayerStackLayer>> LayersBuffer;
	};
}