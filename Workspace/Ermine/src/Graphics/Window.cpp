#include "stdafx.h"
#include "Window.h"

//This Is The Reason Why This Class Is Best Treated As An OpengL Version of The Window...
#include "glad/glad.h"
#include "GLFW/glfw3.h"

Ermine::Window::Window(std::string WindowTitle, std::pair<int, int> WindowDiamensions)
{
    /* Initialize the library */
    if (!glfwInit())
    {
        STDOUTDefaultLog_Error("Failed To Initilize Glfw Hence Quitting Program");
        exit(-1); //Exit The Program If We Fail Something So Vital
    }

    WinPtr = glfwCreateWindow(WindowDiamensions.first, WindowDiamensions.second,
        WindowTitle.c_str(), NULL, NULL);
    //TODO The Window Pointer Must Be Set In The Future..

    if (!WinPtr)
    {
        STDOUTDefaultLog_Error("Failed To Draw Window Hence Quitting Program")
            glfwTerminate();
        exit(-1); //Exit The Program If We Fail Something So Vital
    }
}

Ermine::Window::~Window()
{
    glfwTerminate(); //Glfw Must Be Terminated With The Window.. Though In The Future One May Choose To Move This Into App Class..
}

Ermine::Window::Window(Window&& rhs)
    :
    WinPtr(rhs.WinPtr)
{
    rhs.WinPtr = nullptr;
}

Ermine::Window& Ermine::Window::operator=(Window&& rhs)
{
    WinPtr = rhs.WinPtr;
    rhs.WinPtr = nullptr;

    return *this;
}