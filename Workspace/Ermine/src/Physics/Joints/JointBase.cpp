#include "stdafx.h"
#include "JointBase.h"

#pragma region StaticDefinition
std::atomic<unsigned int> Ermine::JointBase::InstanceCounter = 0;
#pragma endregion StaticDefinition


Ermine::JointBase::JointBase(std::string JointName, b2Body* BodyA, b2Body* BodyB)
    :
    Name(JointName),
    BodyA(BodyA),
    BodyB(BodyB),
    UniqueIdentifier(InstanceCounter.fetch_add(1))
{}

Ermine::JointBase::~JointBase()
{
    if (GetHealth() != Ermine::JointHealthEnum::StatusOk)
    {
        Universum->DestroyJoint(JointHandle);
        JointHandle = nullptr;
    }
}


Ermine::JointBase::JointBase(JointBase&& rhs)
{
    BodyA = rhs.BodyA;
    BodyB = rhs.BodyB;

    rhs.BodyA = nullptr;
    rhs.BodyB = nullptr;

    rhs.Health = Ermine::JointHealthEnum::MarkedForDeletion;
    UniqueIdentifier = rhs.UniqueIdentifier;

    rhs.UniqueIdentifier = 0;

    Name = std::move(rhs.Name);
    rhs.Name = "ERROR";
}
Ermine::JointBase& Ermine::JointBase::operator=(JointBase&& rhs)
{
    BodyA = rhs.BodyA;
    BodyB = rhs.BodyB;

    rhs.BodyA = nullptr;
    rhs.BodyB = nullptr;

    rhs.Health = Ermine::JointHealthEnum::MarkedForDeletion;
    UniqueIdentifier = rhs.UniqueIdentifier;

    rhs.UniqueIdentifier = 0;
    
    Name = std::move(rhs.Name);
    rhs.Name = "ERROR";

    return *this;
}


void Ermine::JointBase::ShiftOrigin(glm::vec2 NewOrigin)
{
    if (GetHealth() == Ermine::JointHealthEnum::StatusOk)
    {
        //This Hopefully Is Ermine::coordErmineToWorld.. I Am Writing This Almost Offhanded.. Do Check It At A Future Date..
        b2Vec2 Origin = Ermine::GLMToB2Vec2(Ermine::coordErmineToWorld(NewOrigin));
        JointHandle->ShiftOrigin(Origin);
    }
    else STDOUTDefaultLog_Error("Could Not Shift Origin Of The Body As The Health Of The Joint Is Not Okay");
}

void Ermine::JointBase::DestroyJoint()
{
    if (Health == Ermine::JointHealthEnum::StatusOk)
    {
        Ermine::Universum->DestroyJoint(JointHandle);
        Health = Ermine::JointHealthEnum::MarkedForDeletion;
    }
    else STDOUTDefaultLog_Trace("Tried To Destroy A Joint Which Was Already Destroyed.. Soo Skipping It..");
}

unsigned int Ermine::JointBase::GetUniqueIdentifier()
{
    return UniqueIdentifier;
}

glm::vec2 Ermine::JointBase::GetBodyAWorldAnchorLocation()
{
    if(GetHealth() == Ermine::JointHealthEnum::StatusOk)
        return  Ermine::coordWorldToErmine(Ermine::B2Vec2ToGLM(JointHandle->GetAnchorA()));
    else
    {
        STDOUTDefaultLog_Error("Could Not Query Body A World Anchor Location As The Health Of The Joint Is Not Okay");
        return glm::vec2(-9999.0f, -9999.0f);
    }
}

glm::vec2 Ermine::JointBase::GetBodyBWorldAnchorLocation()
{
    if (GetHealth() == Ermine::JointHealthEnum::StatusOk)
        return  Ermine::coordWorldToErmine(Ermine::B2Vec2ToGLM(JointHandle->GetAnchorB()));
    else
    {
        STDOUTDefaultLog_Error("Could Not Query Body A World Anchor Location As The Health Of The Joint Is Not Okay");
        return glm::vec2(-9999.0f, -9999.0f);
    }
}

void Ermine::JointBase::SetUserData(void* Data)
{
    if (GetHealth() == Ermine::JointHealthEnum::StatusOk)
        JointHandle->SetUserData(Data);
    else STDOUTDefaultLog_Error("Could Not Set User Data As The Health Of The Joint Is Not Okay");
}
void* Ermine::JointBase::GetUserData()
{
    if (GetHealth() == Ermine::JointHealthEnum::StatusOk)
        return JointHandle->GetUserData();
    else
    {
        STDOUTDefaultLog_Error("Could Not Get User Data As The Health Of The Joint Is Not Okay");
        return nullptr;
    }
}