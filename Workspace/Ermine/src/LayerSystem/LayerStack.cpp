#include "stdafx.h"
#include "LayerStack.h"

#include "2DPrimitives/TileMapLayerRenderable.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

Ermine::LayerStack::LayerStack(std::string Name)
	:
	LayerStackName(Name)
{
	//Subscribed To Concrete Event Default..
	/*Ermine::RecieverComponent::Bind(GenCallableFromMethod(&Ermine::LayerStack::RecievedEvent), RecieveConcreteEvents,
									EventType::ConcreteEvent);*/
}

Ermine::LayerStack::~LayerStack()
{
	for (auto i : AllLayersAssociated)
		delete i;
}

Ermine::LayerStack::LayerStack(const LayerStack& rhs)
{
	HelperCopyConstructor(rhs);
}
Ermine::LayerStack Ermine::LayerStack::operator=(const LayerStack& rhs)
{
	HelperCopyConstructor(rhs);
	return *this;
}

Ermine::LayerStack::LayerStack(LayerStack&& rhs)
{
	HelperMoveConstructor(std::move(rhs));
}
Ermine::LayerStack Ermine::LayerStack::operator=(LayerStack&& rhs)
{
	HelperMoveConstructor(std::move(rhs));
	return *this;
}

void Ermine::LayerStack::PushLayerOntoStackFront(std::unique_ptr<Ermine::LayerStackLayer> LayerToPush)
{
	LayerStackLayer* freepointer = LayerToPush.release();
	AllLayersAssociated.emplace_back(freepointer); //Added On The Top,Top Being the highest index..
}

void Ermine::LayerStack::PushLayerOntoStackAtPosition(std::unique_ptr<Ermine::LayerStackLayer> LayerToPush, int index)
{
	if (index < AllLayersAssociated.size())
	{
		LayerStackLayer* freepointer = LayerToPush.release();
		AllLayersAssociated.insert(AllLayersAssociated.begin() + index, freepointer); //Added On The Index See That Index Is Valid
	}
	else
	{
		PushLayerOntoStackFront(std::move(LayerToPush));
	}
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

void Ermine::LayerStack::Clear()
{
	for (auto& i : AllLayersAssociated)
	{
		delete i;
		i = nullptr;
	}

	AllLayersAssociated.clear();
}

void Ermine::LayerStack::SubmitTileMapForDrawing(Ermine::TileMap const* Tm)
{
	auto TextureCacheGlobal = Ermine::GlobalTextureCache::Get();

	for (int i = 0; i < Tm->Layers.size(); i++)
	{
		Ermine::TileMap::Layer Layer = (Tm->Layers[i]);
		auto Pair = Tm->CreateVertexArrayForLayer(Layer);
		
		Pair.first.SetVertexAttribArray({
				{3,GL_FLOAT,false},
				{2,GL_FLOAT,false},
				{1,GL_FLOAT,false}
			});

		Renderable2D* Obj = new TileMapLayerRenderable(Pair.first, std::move(Material("Shader/TileMapLayerBaseMaterial.json")), Pair.second);

		std::unique_ptr<LayerStackLayer> L = std::make_unique<LayerStackLayer>(Layer.Name);
		L->SubmitRenderable(Obj);
		this->PushLayerOntoStackFront(std::move(L));
		
		delete Obj;
	}
}



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


void Ermine::LayerStack::HelperCopyConstructor(const LayerStack& rhs)
{
	LayerStackName = rhs.LayerStackName;

	for (auto i : rhs.AllLayersAssociated)
		AllLayersAssociated.emplace_back(new LayerStackLayer(*i));
	
	RecieveConcreteEvents = rhs.RecieveConcreteEvents.load();
}

void Ermine::LayerStack::HelperMoveConstructor(LayerStack&& rhs)
{
	LayerStackName = rhs.LayerStackName;

	for (auto& i : rhs.AllLayersAssociated)
	{
		AllLayersAssociated.emplace_back(i);
		i = nullptr;
	}

	RecieveConcreteEvents = rhs.RecieveConcreteEvents.load();
}
