#include "stdafx.h"
#include "LayerStackLayer.h"

Ermine::LayerStackLayer::LayerStackLayer(std::string Name)
	:
	LayerName(Name)
{}

Ermine::LayerStackLayer::~LayerStackLayer()
{
	for (auto i : Renderables)
		delete i;
}

Ermine::LayerStackLayer::LayerStackLayer(const LayerStackLayer& rhs)
{
	HelperCopyConstructor(rhs);
}
Ermine::LayerStackLayer Ermine::LayerStackLayer::operator=(const LayerStackLayer& rhs)
{
	HelperCopyConstructor(rhs);
	return *this;
}

Ermine::LayerStackLayer::LayerStackLayer(LayerStackLayer&& rhs)
{
	HelperMoveConstructor(std::move(rhs));
}
Ermine::LayerStackLayer Ermine::LayerStackLayer::operator=(LayerStackLayer&& rhs)
{
	HelperMoveConstructor(std::move(rhs));
	return *this;
}

void Ermine::LayerStackLayer::SubmitRenderable(Renderable2D* RenderableObj)
{
	HelperEmplaceRenderableInRenderablesContainer(RenderableObj);
}

void Ermine::LayerStackLayer::Clear()
{
	Renderables.clear(); //This is a collection of unique pointers simply calling clear is enough..
}

bool Ermine::LayerStackLayer::HandleConcreteEventInLayerAndPassForward(Ermine::Event* EventPointer)
{
	//This Is The Default Behaviour It Says Pass Forward I Have Nothing To With it
	//Return False If The Layer Has To Stop Propogation..
	return true;
}


void Ermine::LayerStackLayer::HelperCopyConstructor(const LayerStackLayer& rhs)
{
	LayerName = rhs.LayerName;

	for (Renderable2D* i : rhs.Renderables)
		HelperEmplaceRenderableInRenderablesContainer(i);
}

void Ermine::LayerStackLayer::HelperMoveConstructor(LayerStackLayer&& rhs)
{
	LayerName = std::move(rhs.LayerName);

	for (auto& i : rhs.Renderables)
	{
		Renderables.emplace_back(i);
		i = nullptr;
	}
}

void Ermine::LayerStackLayer::HelperEmplaceRenderableInRenderablesContainer(Renderable2D* RenderableObj)
{
	if (dynamic_cast<Actor2D*>(RenderableObj))
		Renderables.emplace_back(new Actor2D(*((Actor2D*)RenderableObj))); //Call The Copy Constructor Essentially..
	else
		Renderables.emplace_back(new Renderable2D(*RenderableObj)); //Dunno What This Is So Just Creating a Renderable Object.. (Note- Renderable Object Does Not Contain Texture Data Keep That In Mind..)
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