#include "stdafx.h"
#include "Object.h"

#include "EventSystem/EnumEventType.h"
#include "EventSystem/EventBroadcastStation.h"
#include "EventSystem/Event.h"

#include "EventSystem/Components/RecieverComponent.h"
#include "EventSystem/Components/BroadcastComponent.h"



#pragma region StaticDeclarations
//Start Static Declarations//
std::atomic<long> Ermine::Object::Counter = 0;

std::unordered_map<std::string, std::shared_ptr<Ermine::Object>> Ermine::Object::FactoryHashTable;
std::recursive_mutex Ermine::Object::FactoryHashTableMutex;
//Ended Static Declarations//
#pragma endregion

void Ermine::Object::FlushUnusedObjects()
{
	//Get The Mutex Inorder To Access FactoryHashTable..
	std::unique_lock<std::recursive_mutex> Lock(FactoryHashTableMutex);

	//Buffer To Hold Marked Objects..
	std::vector<std::string> ErasableStrings;

	//Mark Whatever Needs To Be Erased
	for (auto i = FactoryHashTable.begin(); i != FactoryHashTable.end(); i++)
	{
		if (i->second->GetObjectHealth() == Ermine::ObjectStatus::StatusMarkedForDeletion)
			ErasableStrings.emplace_back(i->first);
	}

	//Erase Whatever Was Marked..
	for (auto str : ErasableStrings)
		FactoryHashTable.erase(str);
}

std::shared_ptr<Ermine::Object> Ermine::Object::GenerateObject(std::string UniqueIdentifier)
{
	std::unique_lock<std::recursive_mutex> Lock(FactoryHashTableMutex);
	
	auto Iterator = FactoryHashTable.find(UniqueIdentifier);

	if (Iterator != FactoryHashTable.end())
	{
		return Iterator->second;
	}
	else
	{
		std::shared_ptr<Ermine::Object> Obj(new Ermine::Object());
		FactoryHashTable[Obj->GetUniqueIdentifier()] = Obj;
		return Obj;
	}
}

#pragma region Constructors
Ermine::Object::Object()
{
	std::unique_lock<std::recursive_mutex> Gaurd1(ObjectMutex);
	long LC = Counter.fetch_add(1); //I am not entirely sure that the add is completed before mutex i released.. sure enough but not absolutely sure..
	UniqueIdentifier = std::to_string(LC);

	FlagsOfRecievingEvents.resize(Ermine::GetNumberOfEventTypesInExistence(), false);
	TicketsHeldByTheObject.resize(Ermine::GetNumberOfEventTypesInExistence(), nullptr);
}

Ermine::Object::Object(Object& rhs)
{
	std::unique_lock<std::recursive_mutex> Gaurd1(ObjectMutex);
	std::unique_lock<std::recursive_mutex> Gaurd2(rhs.ObjectMutex);

	long LC = Counter.fetch_add(1); //I am not entirely sure that the add is completed before mutex i released.. sure enough but not absolutely sure..
	UniqueIdentifier = std::to_string(LC);

	ObjectHealth = rhs.ObjectHealth;

	//Initilize Both The Vectors..
	FlagsOfRecievingEvents.resize(Ermine::GetNumberOfEventTypesInExistence(), false);
	TicketsHeldByTheObject.resize(Ermine::GetNumberOfEventTypesInExistence(), nullptr);

	for (int i = 0; i < rhs.FlagsOfRecievingEvents.size(); i++)
		RecieveEvents(rhs.FlagsOfRecievingEvents[i], Ermine::EventType(i));
}
Ermine::Object& Ermine::Object::operator=(Object& rhs)
{
	std::unique_lock<std::recursive_mutex> Gaurd1(ObjectMutex);
	std::unique_lock<std::recursive_mutex> Gaurd2(rhs.ObjectMutex);

	long LC = Counter.fetch_add(1); //I am not entirely sure that the add is completed before mutex i released.. sure enough but not absolutely sure..
	UniqueIdentifier = std::to_string(LC);

	ObjectHealth = rhs.ObjectHealth;

	//Initilize Both The Vectors..
	FlagsOfRecievingEvents.resize(Ermine::GetNumberOfEventTypesInExistence(), false);
	TicketsHeldByTheObject.resize(Ermine::GetNumberOfEventTypesInExistence(), nullptr);

	for (int i = 0; i < rhs.FlagsOfRecievingEvents.size(); i++)
		RecieveEvents(rhs.FlagsOfRecievingEvents[i], Ermine::EventType(i));

	return *this;
}

Ermine::Object::Object(Object&& rhs)
{
	std::unique_lock<std::recursive_mutex> Gaurd1(ObjectMutex);
	std::unique_lock<std::recursive_mutex> Gaurd2(rhs.ObjectMutex);

	UniqueIdentifier = rhs.UniqueIdentifier;
	rhs.UniqueIdentifier = std::string("Error No Unique Identifier Detected..");

	ObjectHealth = rhs.ObjectHealth;
	rhs.ObjectHealth = Ermine::ObjectStatus::StatusMarkedForDeletion;

	//Initilize Both The Vectors..
	FlagsOfRecievingEvents.resize(Ermine::GetNumberOfEventTypesInExistence(), false);
	TicketsHeldByTheObject.resize(Ermine::GetNumberOfEventTypesInExistence(), nullptr);

	for (int i = 0; i < rhs.FlagsOfRecievingEvents.size(); i++)
		RecieveEvents(rhs.FlagsOfRecievingEvents[i], Ermine::EventType(i));
}
Ermine::Object& Ermine::Object::operator=(Object&& rhs)
{
	std::unique_lock<std::recursive_mutex> Gaurd1(ObjectMutex);
	std::unique_lock<std::recursive_mutex> Gaurd2(rhs.ObjectMutex);

	UniqueIdentifier = rhs.UniqueIdentifier;
	rhs.UniqueIdentifier = std::string("Error No Unique Identifier Detected..");

	ObjectHealth = rhs.ObjectHealth;
	rhs.ObjectHealth = Ermine::ObjectStatus::StatusMarkedForDeletion;

	//Initilize Both The Vectors..
	FlagsOfRecievingEvents.resize(Ermine::GetNumberOfEventTypesInExistence(), false);
	TicketsHeldByTheObject.resize(Ermine::GetNumberOfEventTypesInExistence(), nullptr);

	for (int i = 0; i < rhs.FlagsOfRecievingEvents.size(); i++)
		RecieveEvents(rhs.FlagsOfRecievingEvents[i], Ermine::EventType(i));

	return *this;
}

#pragma endregion

void Ermine::Object::EventReciever(Ermine::Event* Eve)
{
	auto Lock = GetObjectMutex();

	auto EventType = Eve->GetEventType();

	if (EventType == Ermine::EventType::ConcreteEvent)
	{
		std::string Resource = ((ConcreteEvent*)Eve)->GetMessageBuffer();

		ConcreteEventRecieved(Resource);

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

		KeyCallbackEventRecieved(key, scancode, action, mods);

		if (KeyCallBackEventFunctionPointer != nullptr)
			KeyCallBackEventFunctionPointer(key, scancode, action, mods); //Control Is Given To The User.. Be Very Careful..
	}

	if (EventType == Ermine::EventType::CharacterCallbackEvent)
	{
		int CodePoint = ((CharacterCallbackEvent*)Eve)->GetCodePoint();

		CharacterkeyCallbackEventRecieved(CodePoint);

		if (CharacterCallbackEventFunctionPointer != nullptr)
			CharacterCallbackEventFunctionPointer(CodePoint);
	}

	if (EventType == Ermine::EventType::CursorPositionCallbackEvent)
	{
		CursorPositionCallbackEvent* CursorCallbackEve = ((CursorPositionCallbackEvent*)Eve);

		int xpos = ((CursorPositionCallbackEvent*)Eve)->GetXPos();
		int ypos = ((CursorPositionCallbackEvent*)Eve)->GetYPos();

		CursorPositionUpdateEventRecieved(xpos, ypos);

		if (CursorPositionUpdateEventFunctionPointer != nullptr)
			CursorPositionUpdateEventFunctionPointer(xpos, ypos);
	}

	if (EventType == Ermine::EventType::MouseButtonCallbackEvent)
	{
		MouseButtonCallbackEvent* MEve = (MouseButtonCallbackEvent*)Eve;

		int Button = MEve->GetButton();
		int action = MEve->GetAction();
		int mods = MEve->GetMods();

		MouseButtonCallbackEventRecieved(Button, action, mods);

		if (MouseButtonCallbackFunctionPointer != nullptr)
			MouseButtonCallbackFunctionPointer(Button, action, mods);
	}

	if (EventType == Ermine::EventType::ScrollCallbackEvent)
	{
		ScrollCallbackEvent* SEve = (ScrollCallbackEvent*)Eve;

		double OffsetX = SEve->GetXoffset();
		double OffsetY = SEve->GetYoffset();

		ScrollPositionUpdateEventRecieved(OffsetX, OffsetY);

		if (ScrollPositionUpdateFunctionPointer != nullptr)
			ScrollPositionUpdateFunctionPointer(OffsetX, OffsetY);
	}

	if (EventType == Ermine::EventType::OnTickEvent)
	{
		float DeltaTime = ((Ermine::OnTickEvent*)Eve)->GetDeltaTime();

		OnTickEventRecieved(DeltaTime);

		if (OnTickEventFunctionPointer != nullptr)
			OnTickEventFunctionPointer(DeltaTime);
	}

	if (EventType == Ermine::EventType::TileSelectedEvent)
	{
		Ermine::TileSelectedEvent* TEve = (Ermine::TileSelectedEvent*)Eve;

		std::filesystem::path TilesetPath = TEve->GetTilesetPath();
		int Index = TEve->GetIndex();

		TileSelectedUpdateEventRecieved(TilesetPath, Index);

		if (TileSelectedUpdateFunctionPointer != nullptr)
			TileSelectedUpdateFunctionPointer(TilesetPath, Index);
	}
}

void Ermine::Object::RecieveEvents(bool Recieve, Ermine::EventType Type)
{
	if (Recieve == true)
	{
		if (FlagsOfRecievingEvents[(unsigned int)Type] == true)
			return;
		if (FlagsOfRecievingEvents[(unsigned int)Type] == false)
		{
			//Now Subscribe To Station Here
			TicketsHeldByTheObject[(unsigned int)Type] = new Ermine::SubscriptionTicket(std::move(Ermine::RecieverComponent::Bind(GenCallableFromMethod(&Object::EventReciever),
																		   ObjectReadyToRecieveEvents, Type, Object::GenerateObject(this->UniqueIdentifier))));
			FlagsOfRecievingEvents[(unsigned int)Type] = true;
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
