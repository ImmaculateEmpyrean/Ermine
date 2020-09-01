#include "stdafx.h"
#include "JointBase.h"

#pragma region StaticDefinition
unsigned int Ermine::JointBase::InstanceCounter = 0;
#pragma endregion StaticDefinition

Ermine::JointBase::JointBase(b2Body* BodyA, b2Body* BodyB)
    :
    BodyA(BodyA),
    BodyB(BodyB),
    UniqueIdentifier(InstanceCounter++)
{}

Ermine::JointBase::~JointBase()
{
    //This Does Not Do Anything As Of Yet.. Only Exists So That It Can Be Overriden By Its Children..
}


Ermine::JointBase::JointBase(JointBase&& rhs)
{
    HelperMoveConstructor(std::move(rhs));
}
Ermine::JointBase& Ermine::JointBase::operator=(JointBase&& rhs)
{
    HelperMoveConstructor(std::move(rhs));
    return *this;
}


unsigned int Ermine::JointBase::GetUniqueIdentifier()
{
    return UniqueIdentifier;
}

void Ermine::JointBase::SetUserData(void* Data)
{
    if(JointHandle != nullptr)
        JointHandle->SetUserData(Data);
}
void* Ermine::JointBase::GetUserData()
{
    if (JointHandle != nullptr)
        return JointHandle->GetUserData();
    else return nullptr;
}

void Ermine::JointBase::HelperMoveConstructor(Ermine::JointBase&& rhs)
{
    BodyA = std::move(rhs.BodyA);
    BodyB = std::move(rhs.BodyB);

    UniqueIdentifier = rhs.UniqueIdentifier;
}
