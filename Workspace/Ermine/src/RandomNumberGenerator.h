#pragma once
#include <iostream>
#include <random>
#include<cassert>

#include<thread>
#include<mutex>

namespace Ermine
{
    class RandomNumber
    {
    private:
        //A Constructor Taking No Parameters Can Exist Just That The Class Cant Be Constructed Default.. 
        RandomNumber();
        
    public:
        //Note- Range Begin Is Included Range End Is Not..
        static int GetNumberInRange(int RangeBegin, int RangeEnd);

    public:

    protected:

    protected:

    private:
        //This Function Is Used To Get The Singleton..
        static RandomNumber* Get();

    private:

        static std::once_flag InitializationFlag;
        static RandomNumber* Rand;

        std::random_device rd;
        std::mt19937 gen;
    };

//This Is Merely a Utility Macro Using It Or Not Is Arbitrary..
#define ER_RAND_INT(x,y) Ermine::RandomNumber::GetNumberInRange(x,y)
}