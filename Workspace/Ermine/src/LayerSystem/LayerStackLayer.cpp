#include "stdafx.h"
#include "LayerStackLayer.h"

#include "2DPrimitives/Constructs/VertexTextured.h"

Ermine::LayerStackLayer::LayerStackLayer(std::string Name)
	:
	LayerName(Name)
{}

Ermine::LayerStackLayer::~LayerStackLayer()
{
	//The Renderables Buffer Holds SharedPointers Hence No Need To Manually Delete Anyrhing..
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

void Ermine::LayerStackLayer::AddLabel(std::string Text, glm::vec3 Color, glm::vec3 PositionInScreenCoordinates, std::string FontName)
{
	//This Still Not Function Please Refrain From Using This..
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

	for (std::shared_ptr<Ermine::Renderable2D> i : rhs.Renderables)
		HelperEmplaceRenderableInRenderablesContainer(&(*i));
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

/*void Ermine::LayerStackLayer::HelperEmplaceRenderableInRenderablesContainer(std::shared_ptr<Renderable2D> RenderableObj)
{
	
}*/

void Ermine::LayerStackLayer::HelperEmplaceRenderableInRenderablesContainer(Renderable2D* RenderableObj)
{
	if (RenderableObj != nullptr)
	{
		if (dynamic_cast<Actor2D*>(RenderableObj))
			Renderables.emplace_back(new Actor2D(*((Actor2D*)RenderableObj))); //Call The Copy Constructor Essentially..
		else if (dynamic_cast<TileMapLayerRenderable*>(RenderableObj))
			Renderables.emplace_back(new TileMapLayerRenderable(*((TileMapLayerRenderable*)RenderableObj))); //Call The Copy Constructor Essentially..
		else if (dynamic_cast<Label*>(RenderableObj))
			Renderables.emplace_back(new Label(*((Label*)RenderableObj))); //Call The Copy Constructor Essentially..
		else if (dynamic_cast<RenderablePhysicsComponent2D*>(RenderableObj))
			Renderables.emplace_back(new RenderablePhysicsComponent2D(*((RenderablePhysicsComponent2D*)RenderableObj))); //Call The Copy Constructor Essentially..
		else if (dynamic_cast<PhysicsActor*>(RenderableObj))
		{
			//Get It Into A Physics Actor So That We Can Know Its Diamensions..
			PhysicsActor* PhyAct = dynamic_cast<PhysicsActor*>(RenderableObj);

			//Actually Get It Into A PhysicsComponent2D Pointer Now..
			PhysicsComponent2D* Comp = dynamic_cast<PhysicsComponent2D*>(RenderableObj);

			//Get The Translation Matrix which Is To be Sent Into The Gpu For Translating The Object With Respect To The World..
			glm::mat4 TranslationMatrix = Comp->GetTranslationMatrix();

			//Get A Copy Of The RenderableTextureModule To Be Sent So As To Create A Physics Renderable..
			RenderableTextureModule* RenderableCopy = new RenderableTextureModule(*(RenderableTextureModule*)RenderableObj);

			//Set The Vertex Array To Be The One Generated By The Physics Actor Taking Into Account The Diamension Of The Physics Object In The Physics World..
			RenderableCopy->GetVertexArray()->SetVertexBufferData(PhyAct->CalculateModelSpaceVertexes());

			//CReate A RenderablePhysicsComponent Which Can Cleanly Be deleted By The Renderer Once Its Usuage Is Done
			Renderables.emplace_back(new RenderablePhysicsComponent2D(std::move(*RenderableCopy), TranslationMatrix));

			//The Renderable2D is Already Moved Into The Renderables Buffer So Just Go Ahead And Delete This Husk..
			delete RenderableCopy;
		}
		else if (dynamic_cast<Actor2DShape*>(RenderableObj))
			Renderables.emplace_back(new Actor2DShape(*((Actor2DShape*)RenderableObj))); //Call The Copy Constructor Essentially..
		else
		{
			Renderables.emplace_back(new Renderable2D(*RenderableObj)); //Dunno What This Is So Just Creating a Renderable Object.. (Note- Renderable Object Does Not Contain Texture Data Keep That In Mind..)
			STDOUTDefaultLog_Trace("This Most Probably Shouldnt Be Reached Inside Layer Stack Layer :>");
		}
	}
	else
	{
		STDOUTDefaultLog_Error("Donot Submit NullPointers To Layer Stack Layer");
	}
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