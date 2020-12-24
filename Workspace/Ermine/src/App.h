#pragma once
//This Class Is Responsible For Managing The Application, Window Is But a Part of It..

#include<iostream>

#include<string>
#include<vector>

#include<utility>
#include<memory>

#include "Physics/Physics.h"

#include "LayerSystem/LayerStack.h"
#include "LayerSystem/LayerStackLayer.h"		

#include "Graphics/Window/Window.h"

//Forward Declared
int main(int argc, char* argv[]);

namespace Ermine
{
	//App Must Hold A Pointer To Window.. Hence Its Imperative That We Forward Declare It..
	class Window; 

	class App
	{
	public:
		App(std::string AppTitle, std::pair<int, int> Diamensions);

	public:
		//The Destructor Is Virtual Ass App Is Meant To Be Inherited..
		virtual ~App();

		//An App May Not Be Copied Or Moved..
		App(const App& rhs) = delete; 
		App operator=(const App& rhs) = delete; 

		App(App&& rhs) = delete;
		App operator=(const App&& rhs) = delete;

	public:
		//Adds The layer At The Top if No Index Is Provided..
		void AddLayer(std::unique_ptr<Ermine::LayerStackLayer> Layer, int index = 0);

		//These Functions Are USed To Delete Layers..
		void DeleteLayer(std::string LayerName);
		void DeleteLayer(int IndexNumber);
		void DeleteLayer(); //Deletes The TopMost Layer..

	public:

	protected:

	protected:

	private:
#pragma region Routines
		//The App Routine Calls Update Loop And Render Loop On Seperate Threads..
		void AppRoutine();

		void UpdateLoop();
		void RenderLoop(float DeltaTime);
#pragma endregion

	private:
		Window* ManagedWindow = nullptr;

		std::string AppTitle;
		std::pair<int, int> Diamensions;

		Ermine::LayerStack AppLayerStack;

		bool Quit = false;

		friend int ::main(int argc, char* argv[]);
	};
}