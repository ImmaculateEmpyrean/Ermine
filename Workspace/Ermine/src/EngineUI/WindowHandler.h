#pragma once
#include<iostream>
#include<vector>
#include<string>

#include<memory>
#include<mutex>

#include "PrecursorWindow.h"

namespace Ermine
{
	class App; //Forward Declaration is a Good Practice...

	//This is a Singleton Class Used To Submit To A Stack Which Is Then Drawn To The Screen... (Anything In The Stack GetDrawn)
	class WindowHandler
	{
	private:
		WindowHandler() = default; //I want it to be a singleton held by the app class..

	public:
		//Use These Functions to submit windows into imgui..
		//Front Means at Index End.. It Will Most Likely Be Drawn At The Forefront..
		//Back Means at Index Start (0).. It Will Most Likely Be Drawn At The Back...
		void SubmitWindowFront(std::unique_ptr<PrecursorWindow> WindowToBeSubmitted); 
		void SubmitWindowBack(std::unique_ptr<PrecursorWindow> WindowToBeSubmitted);

		//There Is No Reason To Use This Function As Now Pops From The Front Of The Screen i.e Back Of The Screen
		std::unique_ptr<PrecursorWindow> PopWindowFromStack(); 

		//For This To Work App Must First Be Initialized..
		static WindowHandler* Get() { return GlobalWindowHandler; }

	public:

	protected:

	protected:

	private:
		//This Is Like The Main Function of This Class Must Be Called Every Frame To Have An Effect
		void UpdateDraw();

	private:
		std::vector<std::unique_ptr<Ermine::PrecursorWindow>> WindowsBuffer;
		static WindowHandler* GlobalWindowHandler;
		friend class App;
	};
}