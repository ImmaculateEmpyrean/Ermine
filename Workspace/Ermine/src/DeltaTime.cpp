#include "stdafx.h"

Ermine::DeltaTime::DeltaTime(double Time)
    :
    Time(Time)
{}

double Ermine::DeltaTime::GetSeconds()
{
    return Time;
}

double Ermine::DeltaTime::GetMilliSeconds()
{
    return Time * 1000;
}

Ermine::DeltaTime::operator double()
{
    return Time;
}
