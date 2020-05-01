#include "stdafx.h"
#include "Window.h"

//This Is The Reason Why This Class Is Best Treated As An OpengL Version of The Window...
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "InputSystem/GlfwKeyCallbacks.h"

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
   
    glfwMakeContextCurrent(WinPtr); //Made The Created Window Current Context..


    if (!WinPtr)
    {
        STDOUTDefaultLog_Error("Failed To Draw Window Hence Quitting Program")
        glfwTerminate();
        exit(-1); //Exit The Program If We Fail Something So Vital
    }

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        STDOUTDefaultLog_Error("Failed To Initialize GLAD Hence Quitting Program..")
        glfwTerminate();
        exit(-1); //Exit The Program If we Fail Something So Vital
    }

    //Start Set Callbacks To Sense Events...

    glfwSetKeyCallback(WinPtr, key_callback);

    //Ended Set Callbacks To Sense Events...
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

void Ermine::Window::PollEvents()
{
    glfwPollEvents();
}

void Ermine::Window::SwapBuffers()
{
    glfwSwapBuffers(WinPtr);
}

void Ermine::Window::ClearColorBufferBit()
{
    glClear(GL_COLOR_BUFFER_BIT);
}
