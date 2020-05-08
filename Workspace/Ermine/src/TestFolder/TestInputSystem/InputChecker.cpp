#include "stdafx.h"
#include "InputChecker.h"

#include "imgui.h"

Test::InputChecker::InputChecker()
{
	std::cout << "Ysdfjkf";
	Ermine::RecieverComponent::Bind(GenCallableFromMethod(&InputChecker::CallBackFunc), Flag, Ermine::EventType::ConcreteEvent);
	Ermine::RecieverComponent::Bind(GenCallableFromMethod(&InputChecker::CallBackFunc), Flag, Ermine::EventType::KeyCallbackEvent);
	Ermine::RecieverComponent::Bind(GenCallableFromMethod(&InputChecker::CallBackFunc), Flag, Ermine::EventType::CharacterCallbackEvent);
	Ermine::RecieverComponent::Bind(GenCallableFromMethod(&InputChecker::CallBackFunc), Flag, Ermine::EventType::CursorPositionCallbackEvent);
	Ermine::RecieverComponent::Bind(GenCallableFromMethod(&InputChecker::CallBackFunc), Flag, Ermine::EventType::MouseButtonCallbackEvent);
	Ermine::RecieverComponent::Bind(GenCallableFromMethod(&InputChecker::CallBackFunc), Flag, Ermine::EventType::ScrollCallbackEvent);
}

void Test::InputChecker::CallBackFunc(Ermine::Event* eveobj)
{
	auto eventtype = eveobj->GetEventType();

	switch (eventtype)
	{
	case Ermine::EventType::KeyCallbackEvent: STDOUTDefaultLog_Critical("Logged Key Call Back");
		break;
	case Ermine::EventType::CharacterCallbackEvent: STDOUTDefaultLog_Critical("Character Callback Event");
		break;
	case Ermine::EventType::CursorPositionCallbackEvent: STDOUTDefaultLog_Critical("Cursor Key Call Back");
		break;
	case Ermine::EventType::MouseButtonCallbackEvent: STDOUTDefaultLog_Critical("Mouse Button Key Call Back");
		break;
	case Ermine::EventType::ScrollCallbackEvent: STDOUTDefaultLog_Critical("Scroll Key Call Back");
		break;
	default: STDOUTDefaultLog_Error("Unknown CallBAck Recieved Dunno What To Do..");
	}

	/*if (eventtype == Ermine::EventType::CharacterCallbackEvent)
	{
		auto l = (Ermine::CharacterCallbackEvent*)eveobj;
		ImGuiIO& io = ImGui::GetIO();
		io.AddInputCharacter(l->GetCodePoint());
	}*/
}
