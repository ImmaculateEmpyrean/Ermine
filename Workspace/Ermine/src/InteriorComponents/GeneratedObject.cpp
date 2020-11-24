#include "stdafx.h"
#include "GeneratedObject.h"

#include "EventSystem/EnumEventType.h"
#include "EventSystem/EventBroadcastStation.h"
#include "EventSystem/Event.h"

#include "EventSystem/Components/RecieverComponent.h"
#include "EventSystem/Components/BroadcastComponent.h"

#include "Object.h"

#pragma region StaticDeclarations
//Start Static Declarations//
std::atomic<long> Ermine::GeneratedObject::Counter = 0;
//Ended Static Declarations//
#pragma endregion

#pragma region Constructors
Ermine::GeneratedObject::GeneratedObject()
{
	std::unique_lock<std::recursive_mutex> Gaurd1(ObjectMutex);
	long LC = Counter.fetch_add(1); //I am not entirely sure that the add is completed before mutex i released.. sure enough but not absolutely sure..
	UniqueIdentifier = std::to_string(LC);

	FlagsOfRecievingEvents.resize(Ermine::GetNumberOfEventTypesInExistence(), false);
	TicketsHeldByTheObject.resize(Ermine::GetNumberOfEventTypesInExistence(), nullptr);
}


#pragma endregion

#pragma region QueryObjectHealth
Ermine::ObjectStatus Ermine::GeneratedObject::GetObjectHealth()
{
	auto Lock = GetObjectMutex();
	return HObject->GetObjectHealth();
}
void Ermine::GeneratedObject::SetObjectHealth(Ermine::ObjectStatus Status)
{
	auto LOck = GetObjectMutex();
	HObject->SetObjectHealth(Status);
}

void Ermine::GeneratedObject::MarkObjectForDeletion()
{
	auto LOck = GetObjectMutex();
	HObject->SetObjectHealth(Ermine::ObjectStatus::StatusMarkedForDeletion);
}
//This Is A Dangerous Function.. Use With CAUTION 
void Ermine::GeneratedObject::RestoreObjectHealth()
{
	auto Lock = GetObjectMutex();
	HObject->SetObjectHealth(Ermine::ObjectStatus::StatusOk);
}
#pragma endregion

void Ermine::GeneratedObject::EventReciever(Ermine::Event* Eve)
{
	std::unique_lock<std::recursive_mutex> Lock = std::unique_lock<std::recursive_mutex>(ObjectMutex);

	if (HObject->QueryPushEventsToFunctionFlag())
	{
		auto Func = HObject->GetPushToFunction();
		Func(Eve);

		return;
	}

	auto EventType = Eve->GetEventType();

	if (EventType == Ermine::EventType::ConcreteEvent)
	{
		std::string Resource = ((ConcreteEvent*)Eve)->GetMessageBuffer();

		if(HandleValid)
			HObject->ConcreteEventRecieved(Resource);

		if (ConcreteEventFunctionPointer != nullptr)
			ConcreteEventFunctionPointer(Resource); //Control Is Given To The User.. Be Very Careful..
	}

	if (EventType == Ermine::EventType::KeyCallbackEvent)
	{
		KeyCallbackEvent* KeyCallbackEve = ((KeyCallbackEvent*)Eve);

		int key = KeyCallbackEve->GetKey();
		int scancode = KeyCallbackEve->GetScancode();
		int action = KeyCallbackEve->GetAction();
		int mods = KeyCallbackEve->GetMods();

		if (HandleValid)
			HObject->KeyCallbackEventRecieved(key, scancode, action, mods);

		if (KeyCallBackEventFunctionPointer != nullptr)
			KeyCallBackEventFunctionPointer(key, scancode, action, mods); //Control Is Given To The User.. Be Very Careful..
	}

	if (EventType == Ermine::EventType::CharacterCallbackEvent)
	{
		int CodePoint = ((CharacterCallbackEvent*)Eve)->GetCodePoint();

		if (HandleValid)
			HObject->CharacterkeyCallbackEventRecieved(CodePoint);

		if (CharacterCallbackEventFunctionPointer != nullptr)
			CharacterCallbackEventFunctionPointer(CodePoint);
	}

	if (EventType == Ermine::EventType::CursorPositionCallbackEvent)
	{
		CursorPositionCallbackEvent* CursorCallbackEve = ((CursorPositionCallbackEvent*)Eve);

		int xpos = ((CursorPositionCallbackEvent*)Eve)->GetXPos();
		int ypos = ((CursorPositionCallbackEvent*)Eve)->GetYPos();

		if (HandleValid)
			HObject->CursorPositionUpdateEventRecieved(xpos, ypos);

		if (CursorPositionUpdateEventFunctionPointer != nullptr)
			CursorPositionUpdateEventFunctionPointer(xpos, ypos);
	}

	if (EventType == Ermine::EventType::MouseButtonCallbackEvent)
	{
		MouseButtonCallbackEvent* MEve = (MouseButtonCallbackEvent*)Eve;

		int Button = MEve->GetButton();
		int action = MEve->GetAction();
		int mods = MEve->GetMods();

		if (HandleValid)
			HObject->MouseButtonCallbackEventRecieved(Button, action, mods);

		if (MouseButtonCallbackFunctionPointer != nullptr)
			MouseButtonCallbackFunctionPointer(Button, action, mods);
	}

	if (EventType == Ermine::EventType::ScrollCallbackEvent)
	{
		ScrollCallbackEvent* SEve = (ScrollCallbackEvent*)Eve;

		double OffsetX = SEve->GetXoffset();
		double OffsetY = SEve->GetYoffset();

		if (HandleValid)
			HObject->ScrollPositionUpdateEventRecieved(OffsetX, OffsetY);

		if (ScrollPositionUpdateFunctionPointer != nullptr)
			ScrollPositionUpdateFunctionPointer(OffsetX, OffsetY);
	}

	if (EventType == Ermine::EventType::OnRenderTickEvent)
	{
		float DeltaTime = ((Ermine::OnRenderTickEvent*)Eve)->GetDeltaTime();

		if (HandleValid)
			HObject->OnRenderTickEventRecieved(DeltaTime);

		if (OnRenderTickEventFunctionPointer != nullptr)
			OnRenderTickEventFunctionPointer(DeltaTime);
	}

	if (EventType == Ermine::EventType::OnUpdateTickEvent)
	{
		if (HandleValid)
			HObject->OnUpdateTickEventRecieved();

		if (OnUpdateTickEventFunctionPointer != nullptr)
			OnUpdateTickEventFunctionPointer();
	}

	if (EventType == Ermine::EventType::OnBeginEvent)
	{
		std::shared_ptr<void*> Package = ((Ermine::OnBeginEvent*)Eve)->GetHeldPacket();

		if (HandleValid)
			HObject->OnBeginEvent(Package);

		if (OnBeginEventFunctionPointer != nullptr)
			OnBeginEventFunctionPointer(Package);
	}

	if (EventType == Ermine::EventType::TileSelectedEvent)
	{
		Ermine::TileSelectedEvent* TEve = (Ermine::TileSelectedEvent*)Eve;

		std::filesystem::path TilesetPath = TEve->GetTilesetPath();
		int Index = TEve->GetIndex();

		if (HandleValid)
			HObject->TileSelectedUpdateEventRecieved(TilesetPath, Index);

		if (TileSelectedUpdateFunctionPointer != nullptr)
			TileSelectedUpdateFunctionPointer(TilesetPath, Index);
	}
}

void Ermine::GeneratedObject::RecieveEvents(bool Recieve, Ermine::EventType Type)
{
	auto Lock = GetObjectMutex();

	if (Recieve == true)
	{
		if (FlagsOfRecievingEvents[(unsigned int)Type] == true)
			return;
		if (FlagsOfRecievingEvents[(unsigned int)Type] == false)
		{
			if (HandleValid)
			{
				//Now Subscribe To Station Here
				TicketsHeldByTheObject[(unsigned int)Type] = new Ermine::SubscriptionTicket(std::move(Ermine::RecieverComponent::Bind(GenCallableFromMethod(&GeneratedObject::EventReciever),
					ObjectReadyToRecieveEvents, Type, HObject->GetSharedPtrToObject())));
				FlagsOfRecievingEvents[(unsigned int)Type] = true;
			}
		}
	}
	else
	{
		if (FlagsOfRecievingEvents[(unsigned int)Type] == true)
		{
			//Desubscribe To Station Here..
			auto Station = Ermine::EventBroadcastStation::GetStation();
			Station->DestroySubscription(std::move(*TicketsHeldByTheObject[(unsigned int)Type]));

			//Delete The Allocated Space Even If The Variable Is Moved As Per The Norm..
			delete TicketsHeldByTheObject[(unsigned int)Type];

			FlagsOfRecievingEvents[(unsigned int)Type] = false;
		}
		if (FlagsOfRecievingEvents[(unsigned int)Type] == false)
			return;
	}
}


void Ermine::GeneratedObject::BindObject(Ermine::Object* Handle)
{
	auto Lock = GetObjectMutex();

	HObject = Handle;
	HandleValid = true; //There IS No Way To Truly Know If An Object Is Valid.. Just Because This Flag Is Set Does Not Mean Everything Is Hunky Dory..
}

void Ermine::GeneratedObject::UnBindObject()
{
	auto Lock = GetObjectMutex();

	HObject = nullptr;
	HandleValid = false;
}

