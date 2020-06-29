#pragma once

namespace Ermine
{
	enum class EventType : unsigned int
	{
		ConcreteEvent = 1,
		KeyCallbackEvent = 2,
		CharacterCallbackEvent=3,
		CursorPositionCallbackEvent=4,
		MouseButtonCallbackEvent=5,
		ScrollCallbackEvent=6,
		TileSelectedEvent=7
	};
}