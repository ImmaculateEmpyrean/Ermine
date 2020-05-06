#include "stdafx.h"
#include "InputInterrogator.h"

#include "ErmineKeyCodesDefinition.h"

namespace Ermine
{
	InputInterrogator::InputInterrogator()
	{
		KeyPressedBuffer.resize(1024, false);
		RecieverComponent::Bind(GenCallableFromMethod(&InputInterrogator::RecieveKeyPressedEvent), RecieveEventsFlag, EventType::KeyCallbackEvent);
	}

	bool InputInterrogator::IsKeyPressed(int KeyCode)
	{
		std::lock_guard<std::mutex>LockAccessToKeyPressedBuffer(KeyPressedBufferMutex, std::adopt_lock);
		return KeyPressedBuffer[KeyCode];
	}

	void InputInterrogator::RecieveKeyPressedEvent(Event* eve)
	{
		Ermine::KeyCallbackEvent* point = (Ermine::KeyCallbackEvent*)eve;
		if (point->GetAction() == ERMINE_PRESS || point->GetAction() == ERMINE_REPEAT)
		{
			std::lock_guard<std::mutex>LockAccessToKeyPressedBuffer(KeyPressedBufferMutex, std::adopt_lock);
			KeyPressedBuffer[point->GetKey()] = true;
		}
		else if (point->GetAction() == ERMINE_RELEASE)
		{
			std::lock_guard<std::mutex>LockAccessToKeyPressedBuffer(KeyPressedBufferMutex, std::adopt_lock);
			KeyPressedBuffer[point->GetKey()] = false;
		}
	}
}