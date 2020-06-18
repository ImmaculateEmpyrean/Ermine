#include "stdafx.h"
#include "LayerStack.h"

Ermine::LayerStack::LayerStack(std::string& Name)
	:
	LayerStackName(Name)
{
	//Subscribed To Concrete Event Default..
	Ermine::RecieverComponent::Bind(GenCallableFromMethod(&Ermine::LayerStack::RecievedEvent), RecieveConcreteEvents,
									EventType::ConcreteEvent);
}

Ermine::LayerStack::~LayerStack()
{
	for (auto i : AllLayersAssociated)
		delete i;
}

void Ermine::LayerStack::PushLayerOntoStackFront(std::unique_ptr<Ermine::LayerStackLayer> LayerToPush)
{
	LayerStackLayer* freepointer = LayerToPush.release();
	AllLayersAssociated.emplace_back(freepointer); //Added On The Top,Top Being the highest index..
}

void Ermine::LayerStack::PushLayerOntoStackAtPosition(std::unique_ptr<Ermine::LayerStackLayer> LayerToPush, int index)
{
	LayerStackLayer* freepointer = LayerToPush.release();
	AllLayersAssociated.insert(AllLayersAssociated.begin() + index, freepointer); //Added On The Index See That Index Is Valid
}

void Ermine::LayerStack::PushLayerOnTheBackOfTheStack(std::unique_ptr<Ermine::LayerStackLayer> LayerToPush)
{
	LayerStackLayer* freepointer = LayerToPush.release();
	AllLayersAssociated.insert(AllLayersAssociated.begin(), freepointer); //Added On The Bottom,Bottom Being the lowest index..
}

#if defined(ER_DEBUG_DEVELOP) || defined(ER_DEBUG_SHIP)
std::optional<Ermine::LayerStackLayer*> Ermine::LayerStack::GetIndexFromName(std::string LayerStackLayerName)
{
	//Note Beaware If Two Layers Have The Same Name The First Which Is Near The Bottom Is Returned..
	//Will Not Bother Improving the function as it is only intended for debug purposes..

	for (auto i : AllLayersAssociated)
	{
		if (LayerStackLayerName == i->LayerName)
			return i;
	}

	//Returns Nothing As This is an optional and we can return nothing "could have returned nullptr to right.."
	return {};
}
#endif

void Ermine::LayerStack::RecievedEvent(Ermine::Event* EventPointer)
{
	STDOUTDefaultLog_Error("Recieved Event To LAyer Stack : {}", EventPointer->GetEventType());
	auto Var = EventPointer->GetEventType();
	switch (Var)
	{
		case Ermine::EventType::ConcreteEvent: RecievedEventConcreteEvent(EventPointer);
											   break;
		default: STDOUTDefaultLog_Error("Unknown Event Type See That Its Rectified Line :{} File: {} ", __LINE__, __FILE__);
	}
}

void Ermine::LayerStack::RecievedEventConcreteEvent(Ermine::Event* EventPointer)
{
	for (int i = AllLayersAssociated.size()-1;i>=0;i--)
	{
		bool ShouldIContinue = AllLayersAssociated[i]->HandleEventInLayerAndPassForward(EventPointer);

		//If The Function Has Returned False It Means It Does Not Want it To Be Propogated Further..
		if (ShouldIContinue == false)
			break;
	}
}
