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

#include "Physics/Physics.h"
#include "Physics/PhysicsWorldInitializationStruct.h"

#include "RandomNumberGenerator.h"

//Forward Declared
int main(int argc, char* argv[]);

namespace Ermine
{
	class Window; //Forward Declared So As To Let The App Declaration Know That Window Exists..

	class App
	{
	private:
		App(std::string AppTitle, std::pair<int, int> Diamensions, PhysicsWorldInitializationStruct PhysicsConfig);

	public:
		~App();

		App(const App& rhs) = delete; //This Is The Main PLease Dont Have Two Of Them Or Something Like That
		App operator=(const App& rhs) = delete; //This Is The Main PLease Dont Have Two Of Them Or Something Like That

		App(App&& rhs) = delete; //There is absolutely no reason to move the app
		App operator=(const App&& rhs) = delete; //There is absolutely no reason to move the app

	public:
		static App* Get();

		//Main Stuff Inside App

#pragma region Routines
		//The App Routine Calls Update Loop And Render Loop On Seperate Threads..
		void AppRoutine();

		void UpdateLoop();
		void RenderLoop(float DeltaTime);
#pragma endregion

	public:
		

	protected:

	protected:

	private:

	private:
		static std::once_flag InitializationFlag;

		Window* ManagedWindow;

		std::string AppTitle;
		std::pair<int, int> Diamensions;

		//ExchangeAppEvents Obj;
		static App* PointerToApp;

		InputInterrogator InpInterrogator;

		bool Quit = false;

		friend int ::main(int argc, char* argv[]);
	};
}