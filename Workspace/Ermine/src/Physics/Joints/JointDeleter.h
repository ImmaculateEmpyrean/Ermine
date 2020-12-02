#pragma once
#include<iostream>
#include<memory>
#include "Physics/Physics.h"

namespace Ermine
{
    template< typename T >
    struct JointDeleter
    {
        void operator ()(T* p)
        {
            if (p->GetHealth() == Ermine::JointHealthEnum::StatusOk)
            {
                p->DestroyJoint();
            }
        }
    };
}