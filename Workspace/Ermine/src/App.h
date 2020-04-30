#pragma once
//This Class Is Responsible For Managing The Application, Window Is But a Part of It..

#include<iostream>

#include<string>
#include<vector>
#include<utility>

#include<memory>

#include "ExchangeStructs/ExchangeAppEvents.h"

namespace Ermine
{
	class App
	{
	public:
		App(std::string AppTitle, std::pair<int, int> Diamensions);
		~App();

		App(const App& rhs) = delete; //This Is The Main PLease Dont Have Two Of Them Or Something Like That
		App operator=(const App& rhs) = delete; //This Is The Main PLease Dont Have Two Of Them Or Something Like That

		App(App&& rhs) = delete; //There is absolutely no reason to move the app
		App operator=(const App&& rhs) = delete; //There is absolutely no reason to move the app

	public:
		void OnAttach(); //This is an event which must be handled by the game..
		void OnTick(); //This is an event which must be handled by the game..
		void OnDetach(); //This is an event which must be handled by the game..

	public:

	protected:

	protected:

	private:
		//This Is A Most Important Function Which Advances The App To The Next Frame To Be Called By Main
		//OnTick Is Presumably called Here..
		void NextFrame(); 

	private:
		std::string AppTitle;
		std::pair<int, int> Diamensions;

		ExchangeAppEvents Obj;
	};
}

//extern Ermine::App* GetAppFromGame();