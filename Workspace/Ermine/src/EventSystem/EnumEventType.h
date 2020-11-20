#pragma once

namespace Ermine
{
	enum class EventType : unsigned int
	{
		ConcreteEvent = 0,
		KeyCallbackEvent = 1,
		CharacterCallbackEvent = 2,
		CursorPositionCallbackEvent = 3,
		MouseButtonCallbackEvent = 4,
		ScrollCallbackEvent = 5,
		TileSelectedEvent = 6,

		OnTickEvent = 7,
		OnBeginEvent = 8
	};

	//Do Not Forget To Update This Function.. When Messing With This File..
	int GetNumberOfEventTypesInExistence(); //This Returns 9 For Now..
}