#pragma once
//This Class Is Responsible For Managing The Application, Window Is But a Part of It..

#include<iostream>

#include<string>
#include<vector>
#include<utility>

#include<memory>

#include "ExchangeStructs/ExchangeAppEvents.h"

#include "EngineUI/WindowHandler.h"

#include "InputSystem/InputInterrogator.h"

//Forward Declared
int main(int argc, char* argv[]);

namespace Ermine
{
	class Window; //Forward Declared So As To Let The App Declaration Know That Window Exists..

	class App
	{
	private:
		App(std::string AppTitle, std::pair<int, int> Diamensions);

	public:
		~App();

		App(const App& rhs) = delete; //This Is The Main PLease Dont Have Two Of Them Or Something Like That
		App operator=(const App& rhs) = delete; //This Is The Main PLease Dont Have Two Of Them Or Something Like That

		App(App&& rhs) = delete; //There is absolutely no reason to move the app
		App operator=(const App&& rhs) = delete; //There is absolutely no reason to move the app

	public:
		void OnAttach(); //This is an event which must be handled by the game..
		void OnTick(); //This is an event which must be handled by the game..
		void OnDetach(); //This is an event which must be handled by the game..

		static App* Get();

	public:

	protected:

	protected:

	private:
		//This Is A Most Important Function Which Advances The App To The Next Frame To Be Called By Main
		//OnTick Is Presumably called Here..
		void NextFrame(); 

	private:
		static std::once_flag InitializationFlag;

		Window* ManagedWindow;

		std::string AppTitle;
		std::pair<int, int> Diamensions;

		ExchangeAppEvents Obj;
		static App* PointerToApp;

		InputInterrogator InpInterrogator;

		friend int ::main(int argc, char* argv[]);
	};
}

//extern Ermine::App* GetAppFromGame();