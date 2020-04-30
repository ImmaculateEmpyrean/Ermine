#pragma once
#include<iostream>
#include<vector>
#include<string>

#include "EventSystem/Event.h"

namespace Ermine
{
	//Forward Declaration For The Purposes Of Being a friend...
	class LayerStack;

	class LayerStackLayer
	{
	public:
		LayerStackLayer(std::string Name);

	public:
		//This method Will return true if the event is To Be Passed Forward else false
		

	public:

	protected:
		virtual bool HandleConcreteEventInLayerAndPassForward(Ermine::Event* EventPointer);

	protected:

	private:
		bool HandleEventInLayerAndPassForward(Ermine::Event* EventPointer);
		
	private:
		std::string LayerName;

		friend class Ermine::LayerStack;

	};
}