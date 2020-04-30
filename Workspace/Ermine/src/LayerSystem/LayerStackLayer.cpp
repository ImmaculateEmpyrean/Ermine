#include "stdafx.h"
#include "LayerStackLayer.h"

Ermine::LayerStackLayer::LayerStackLayer(std::string Name)
	:
	LayerName(Name)
{}

bool Ermine::LayerStackLayer::HandleConcreteEventInLayerAndPassForward(Ermine::Event* EventPointer)
{
	//This Is The Default Behaviour It Says Pass Forward I Have Nothing To With it
	//Return False If The Layer Has To Stop Propogation..
	return true;
}

bool Ermine::LayerStackLayer::HandleEventInLayerAndPassForward(Ermine::Event * EventPointer)
{
	//This Is The Default Implementation It is Expected We create our own If We actually Want To Handle Events
	int ConcreteFlag = 1;

	ConcreteFlag = HandleConcreteEventInLayerAndPassForward(EventPointer);

	//And All Recieved Flags to recieve the final flag then return the final flag
	int FinalFlag = ConcreteFlag;

	return FinalFlag;
}
