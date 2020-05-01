#pragma once
//As Of Writing This Header Files There Are No Plans To Make This Engine Multiple Graphics Api Compatible This is Only Written For OpengL
//As Such There Will Absolutely Be No Window Layer Abstraction Api Written Into This..
//In The Future Should Someone Wish to Make it Cross Graphics Api Compatible Please Write a Window Layer Abstraction Class And If Possible..
//(continued) Use This Class as a form of opengls implementation of the window

#include<iostream>
#include<vector>
#include<string>

#include<utility>

struct GLFWwindow; //Forward Declared So as To Not Include Glad and Glfw..

namespace Ermine
{
	class Window
	{
	public:
		Window(std::string WindowTitle, std::pair<int, int> WindowDiamensions);
		~Window();

		Window(const Window& rhs) = delete; //You Cannot Copy A Window Amigo..
		Window operator=(const Window& rhs) = delete; //You Simply Are Not Going Some Variation Copy..

		Window(Window&& rhs); //Totally possible..
		Window& operator=(Window&& rhs); //totally Acceptable..

	public:
		//This is const so as to not modify the window from outside it is the job of this class
		GLFWwindow const* GetContext() { return WinPtr; }

		//Must Be Called Every Frame To Get Inputs From The User..
		void PollEvents();

		//This In The Future Must Live Inside The Renderer Or Something Like That No Reason To Give This To Window..
		void SwapBuffers();
		void ClearColorBufferBit();

	private:
		GLFWwindow* WinPtr;
	};

}