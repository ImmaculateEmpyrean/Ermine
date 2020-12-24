#include "stdafx.h"
#include "LayerStackLayer.h"

#include "2DPrimitives/Constructs/VertexTextured.h"

#include "Graphics/Renderer/RenderableComponents/Renderable2D.h"

Ermine::LayerStackLayer::LayerStackLayer(std::string Name)
	:
	LayerName(Name)
{}

Ermine::LayerStackLayer::~LayerStackLayer()
{}

#pragma region EventHandlerFunctions
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

	if (EveType == Ermine::EventType::OnUpdateTickEvent)
	{
		//The Recieved EventHandle  Flag Is Discarded For UpdateTick As You Have To Propogate This Event Into All Layers..
		Ermine::OnUpdateTickEvent* UpdateTickEvent = (Ermine::OnUpdateTickEvent*)EventPointer;
		OnUpdateTickEventRecieved(UpdateTickEvent->GetDeltaTime());
	}

	if (EveType == Ermine::EventType::OnRenderTickEvent)
	{
		//The Recieved EventHandle  Flag Is Discarded For RenderTick As You Have To Propogate This Event Into All Layers..
		Ermine::OnRenderTickEvent* RenderTickEvent = (Ermine::OnRenderTickEvent*)EventPointer;
		OnRenderTickEventRecieved(RenderTickEvent->GetDeltaTime());
	}

	return EventHandled;
}
#pragma endregion