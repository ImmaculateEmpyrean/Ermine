#pragma once
#include<iostream>
#include<vector>
#include<string>

#include<functional>

namespace Ermine
{
	class App;
}

struct InitializaDLLStruct
{
	//For Now The Initialization Struct Has Only One Elemnet.. Will Increase In The Future Hopefully..
	Ermine::App* App;
};

//The Function IS Declared Extern As It Is Actually Implemented In The DLL And Exported..
extern void DLL __cdecl InitializeDLL(InitializaDLLStruct Struct);

//This Function Checks If Said App Can Be Accessed Properly
extern void DLL _cdecl Checker();

//This Function Is Called By The App To Initialize Its Renderer..
extern void DLL _cdecl InitializeScene();
