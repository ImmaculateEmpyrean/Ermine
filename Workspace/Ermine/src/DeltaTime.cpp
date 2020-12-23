#include "stdafx.h"

//TODO- Must Get Rid Of These Headers.. They Have No Reason To Be Here..
#include "glad/glad.h"
#include "GLFW/glfw3.h"

double Ermine::DeltaTime::GetSeconds()
{
    std::unique_lock<std::recursive_mutex> Lock(Mut);
    return DTime;
}

double Ermine::DeltaTime::GetMilliSeconds()
{
    std::unique_lock<std::recursive_mutex> Lock(Mut);
    return DTime * 1000;
}

Ermine::DeltaTime::operator double()
{
    std::unique_lock<std::recursive_mutex> Lock(Mut);
    return DTime;
}

void Ermine::DeltaTime::Update()
{
    std::unique_lock<std::recursive_mutex> Lock(Mut);

    float    TimeS = glfwGetTime(); //This Function Must Be Gotten Rid Of If We Are To Get Rid Of The Headers At the Top..
	DTime =  TimeS - Time;
    Time  =  TimeS;
}
