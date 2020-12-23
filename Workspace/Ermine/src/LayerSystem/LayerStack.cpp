#include "stdafx.h"
#include "LayerStack.h"

#include "2DPrimitives/TileMapLayerRenderable.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#pragma region Constructors
Ermine::LayerStack::LayerStack(std::string Name)
	:
	Object(),
	LayerStackName(Name)
{
	auto Lock = Ermine::Object::GetObjectMutex();

	//Create A Function Pointer To The Push To Function.. And Use It To Get Our Events To That Function
	std::function<void(Ermine::Event*)> Func = std::bind(&Ermine::LayerStack::RecieveEvents, this,std::placeholders::_1);
	Object::AssignPushEventsToFunction(Func);

	//Start What All Events Must This Object Recieve..//
	Object::RecieveEvents(true, Ermine::EventType::ConcreteEvent);
	Object::RecieveEvents(true, Ermine::EventType::KeyCallbackEvent);
	Object::RecieveEvents(true, Ermine::EventType::CharacterCallbackEvent);
	Object::RecieveEvents(true, Ermine::EventType::CursorPositionCallbackEvent);
	Object::RecieveEvents(true, Ermine::EventType::MouseButtonCallbackEvent);
	Object::RecieveEvents(true, Ermine::EventType::ScrollCallbackEvent);
	//Ended What All Events Must This Object Recieve..//
}

Ermine::LayerStack::~LayerStack()
{}

Ermine::LayerStack::LayerStack(LayerStack&& rhs)
{
	auto ForeignLock = rhs.GetObjectMutex();
	auto Lock = rhs.GetObjectMutex();

	LayerStackName = std::move(rhs.LayerStackName);
	LayersBuffer = std::move(rhs.LayersBuffer);

	std::function<void(Ermine::Event*)> Func = std::bind(&Ermine::LayerStack::RecieveEvents, this, std::placeholders::_1);
	Object::AssignPushEventsToFunction(Func);

	//Start What All Events Must This Object Recieve..//
	Object::RecieveEvents(true, Ermine::EventType::ConcreteEvent);

	Object::RecieveEvents(true, Ermine::EventType::KeyCallbackEvent);
	Object::RecieveEvents(true, Ermine::EventType::CharacterCallbackEvent);
	Object::RecieveEvents(true, Ermine::EventType::CursorPositionCallbackEvent);
	Object::RecieveEvents(true, Ermine::EventType::MouseButtonCallbackEvent);
	Object::RecieveEvents(true, Ermine::EventType::ScrollCallbackEvent);
}
Ermine::LayerStack& Ermine::LayerStack::operator=(LayerStack&& rhs)
{
	auto ForeignLock = rhs.GetObjectMutex();
	auto Lock = rhs.GetObjectMutex();

	LayerStackName = std::move(rhs.LayerStackName);
	LayersBuffer = std::move(rhs.LayersBuffer);

	std::function<void(Ermine::Event*)> Func = std::bind(&Ermine::LayerStack::RecieveEvents, this, std::placeholders::_1);
	Object::AssignPushEventsToFunction(Func);

	//Start What All Events Must This Object Recieve..//
	Object::RecieveEvents(true, Ermine::EventType::ConcreteEvent);

	Object::RecieveEvents(true, Ermine::EventType::KeyCallbackEvent);
	Object::RecieveEvents(true, Ermine::EventType::CharacterCallbackEvent);
	Object::RecieveEvents(true, Ermine::EventType::CursorPositionCallbackEvent);
	Object::RecieveEvents(true, Ermine::EventType::MouseButtonCallbackEvent);
	Object::RecieveEvents(true, Ermine::EventType::ScrollCallbackEvent);

	return *this;
}
#pragma endregion


#pragma region PushLayerOntoStack
void Ermine::LayerStack::PushLayerOntoStackFront(std::unique_ptr<Ermine::LayerStackLayer> LayerToPush)
{
	auto Lock = GetObjectMutex();
	LayersBuffer.insert(LayersBuffer.begin(), std::move(LayerToPush));
}
void Ermine::LayerStack::PushLayerOnTheBackOfTheStack(std::unique_ptr<Ermine::LayerStackLayer> LayerToPush)
{
	auto LOck = GetObjectMutex();
	LayersBuffer.emplace_back(std::move(LayerToPush));
}
void Ermine::LayerStack::PushLayerOntoStackAtPosition(std::unique_ptr<Ermine::LayerStackLayer> LayerToPush, int index)
{
	auto Lock = GetObjectMutex();
	LayersBuffer.insert(LayersBuffer.begin() + index,std::move(LayerToPush));
}

void Ermine::LayerStack::PushLayerOntoStackFront(std::string LayerName)
{
	auto Lock = GetObjectMutex();
	LayersBuffer.insert(LayersBuffer.begin(), std::make_unique<Ermine::LayerStackLayer>(LayerName));
}
void Ermine::LayerStack::PushLayerOnTheBackOfTheStack(std::string LayerName)
{
	auto Lock = GetObjectMutex();
	LayersBuffer.emplace_back(std::make_unique<Ermine::LayerStackLayer>(LayerName));
}
void Ermine::LayerStack::PushLayerOntoStackAtPosition(std::string LayerName, int index)
{
	auto Lock = GetObjectMutex();
	LayersBuffer.insert(LayersBuffer.begin() + index, std::make_unique<Ermine::LayerStackLayer>(LayerName));
}


int Ermine::LayerStack::GetLayerStackSize()
{
	auto Lock = GetObjectMutex();
	return LayersBuffer.size();
}
#pragma endregion


std::unique_ptr<Ermine::LayerStackLayer> Ermine::LayerStack::GetLayer(std::string LayerName)
{
	auto Lock = GetObjectMutex();

	int Counter = 0;
	for (auto& i : LayersBuffer)
	{
		if (i->GetName() == LayerName)
		{
			std::unique_ptr<Ermine::LayerStackLayer> Ptr = std::move(LayersBuffer[Counter]);
			LayersBuffer.erase(LayersBuffer.begin() + Counter);

			return std::move(Ptr);
		}
		++Counter;
	}
	return nullptr;
}
std::unique_ptr<Ermine::LayerStackLayer> Ermine::LayerStack::GetLayer(int IndexInTheStack)
{
	auto Lock = GetObjectMutex();

	std::unique_ptr<Ermine::LayerStackLayer> Ptr = std::move(LayersBuffer[IndexInTheStack]);
	LayersBuffer.erase(LayersBuffer.begin() + IndexInTheStack);

	return std::move(Ptr);
}


void Ermine::LayerStack::Clear()
{
	auto Lock = GetObjectMutex();
	LayersBuffer.clear();
}


void Ermine::LayerStack::RecieveEvents(Ermine::Event* Eve)
{
	auto Lock = GetObjectMutex();

	for (std::unique_ptr<Ermine::LayerStackLayer>& i : LayersBuffer)
	{
		//Decided Not To Propogate Further If Event is Handled.. i.e return value is true..
		if (i->HandleEvents(Eve))
			return;
	}
}