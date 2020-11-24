#pragma once
#include<iostream>
#include<vector>
#include<string>

namespace Ermine
{
	//Class Declarations Becuse Lets Say The Compliler Needs Them.. X>

	class App;
}

//These Are The Pointers Both Function And Classes Which Can Comfortably Be Used By The Dll.. Infact These Are Initilizad The App Itself Authorizing Usuage..
extern Ermine::App* HApp;