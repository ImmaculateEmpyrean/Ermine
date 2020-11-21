#include "stdafx.h"
#include "LayerStackLayer.h"

#include "2DPrimitives/Constructs/VertexTextured.h"

Ermine::LayerStackLayer::LayerStackLayer(std::string Name)
	:
	LayerName(Name)
{}

Ermine::LayerStackLayer::~LayerStackLayer()
{}

Ermine::LayerStackLayer::LayerStackLayer(LayerStackLayer&& rhs)
{
	LayerName = std::move(rhs.LayerName);
	Renderables = std::move(rhs.Renderables);
}
Ermine::LayerStackLayer& Ermine::LayerStackLayer::operator=(LayerStackLayer&& rhs)
{
	LayerName = std::move(rhs.LayerName);
	Renderables = std::move(rhs.Renderables);

	return *this;
}

void Ermine::LayerStackLayer::SubmitActor(std::shared_ptr<Actor2DBase> Ptr)
{
	//Identify Which Object Was Recieved..
	Ermine::ActorFamilyIdentifier Identifier = Ptr->GetActorFamilyIdentifier();

	if (Identifier == Ermine::ActorFamilyIdentifier::Actor2D)
	{
		auto RenderableTextureModule = RenderableTextureModule::Generate(std::dynamic_pointer_cast<Ermine::Actor2D>(Ptr));
		Renderables.emplace_back(std::move(RenderableTextureModule));
	}
}

void Ermine::LayerStackLayer::AddLabel(std::string Text, glm::vec3 Color, glm::vec3 PositionInScreenCoordinates, std::string FontName)
{
	//This Still Does Not Function Please Refrain From Using This..
}

void Ermine::LayerStackLayer::Clear()
{
	Renderables.clear();
}

std::string Ermine::LayerStackLayer::GetName()
{
	return LayerName;
}

bool Ermine::LayerStackLayer::HandleEvents(Ermine::Event* EventPointer)
{
	Ermine::EventType EveType = EventPointer->GetEventType();
	bool EventHandled = false;

	if (EveType == Ermine::EventType::ConcreteEvent)
	{
		Ermine::ConcreteEvent* CEve = (Ermine::ConcreteEvent*)EventPointer;
		EventHandled = ConcreteEventRecieved(CEve->GetMessageBuffer());
	}
	if (EveType == Ermine::EventType::KeyCallbackEvent)
	{
		Ermine::KeyCallbackEvent* KEve = (Ermine::KeyCallbackEvent*)EventPointer;
		EventHandled = KeyCallbackEventRecieved(KEve->GetKey(),KEve->GetScancode(),KEve->GetAction(),KEve->GetMods());
	}
	if (EveType == Ermine::EventType::CharacterCallbackEvent)
	{
		Ermine::CharacterCallbackEvent* CEve = (Ermine::CharacterCallbackEvent*)EventPointer;
		EventHandled = CharacterkeyCallbackEventRecieved(CEve->GetCodePoint());
	}
	if (EveType == Ermine::EventType::CursorPositionCallbackEvent)
	{
		Ermine::CursorPositionCallbackEvent* CurEve = (Ermine::CursorPositionCallbackEvent*)EventPointer;
		EventHandled = CursorPositionUpdateEventRecieved(CurEve->GetXPos(),CurEve->GetYPos());
	}
	if (EveType == Ermine::EventType::MouseButtonCallbackEvent)
	{
		Ermine::MouseButtonCallbackEvent* MEve = (Ermine::MouseButtonCallbackEvent*)EventPointer;
		EventHandled = MouseButtonCallbackEventRecieved(MEve->GetButton(), MEve->GetAction(), MEve->GetMods());
	}
	if (EveType == Ermine::EventType::ScrollCallbackEvent)
	{
		Ermine::ScrollCallbackEvent* SEve = (Ermine::ScrollCallbackEvent*)EventPointer;
		EventHandled = ScrollPositionUpdateEventRecieved(SEve->GetXoffset(), SEve->GetYoffset());
	}

	return EventHandled;
}

void Ermine::LayerStackLayer::DefaultEventHandler()
{
	STDOUTDefaultLog_Info("Default Handler Of Event Invoked In LayerStackLayer.. Guess You ARe Unnecessarily Wasting Some Calls..");
}