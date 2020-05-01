#pragma once
#include "Core.h"

namespace Ermine
{
	enum class EventType : unsigned int
	{
		ConcreteEvent = 1,
		KeyCallbackEvent = 2,
		CharacterCallbackEvent=3,
		CursorPositionCallbackEvent=4,
		MouseButtonCallbackEvent=5,
		ScrollCallbackEvent=6
	};
}