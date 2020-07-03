#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<memory>
#include<utility>
#include<atomic>
#include<mutex>

#include "EventSystem/Components/RecieverComponent.h"

//This Class Is A Work In Progress Dont Use it As Of Now..

namespace Ermine
{
	class App; //Forward Declaration So That The Following Class Can Become A Friend

	class InputInterrogator
	{
	private:
		InputInterrogator();

	public:
		//bool IsKeyPressed(int KeyCode);

	public:

	protected:

	protected:

	private:
		//void RecieveKeyPressedEvent(Event* eve);

	private:
		std::mutex KeyPressedBufferMutex;
		std::vector<bool> KeyPressedBuffer;

		std::atomic_bool RecieveEventsFlag = true;

		friend class Ermine::App;
	};

}