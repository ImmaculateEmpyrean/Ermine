#include "stdafx.h"
#include "MutexManager.h"

#include "Interfaces/IMutex.h"

//Start Static Variables Declarations..//
Ermine::MutexManager* Ermine::MutexManager::Manager = nullptr;
std::once_flag Ermine::MutexManager::InitializationFlag;
//Ended Static Variables Declarations..//

Ermine::MutexManager* Ermine::MutexManager::GetManager()
{
    std::call_once(InitializationFlag, []() {
        Manager = new MutexManager();
    });

    return Manager;
}

Ermine::MutexState Ermine::MutexManager::AcquireMutex(Ermine::Object* Obj, MutexLevel WhichMutex)
{
    if (GetMutexState(Obj->GetUniqueIdentifier()) != Ermine::MutexState::Okay)
        return Ermine::MutexState::Okay;

    bool FlagToAvoidFlood = false;
    bool FlagToAvoidGettingMutex = false;

    //Please Implement Some Kind Of Check Atleast In The Debug Mode In The Future..
    IMutex* MutexInterface = dynamic_cast<IMutex*>(Obj);
    if (MutexInterface != nullptr)
    {
        Ermine::MutexLevel RecievedLevel = WhichMutex;//MutexInterface->GetMutexLevel();

        auto& Casket = StorageBuffer.find(Obj->GetUniqueIdentifier());

        if (Casket != StorageBuffer.end())
        {
            //Casket Is Found.. //Maybe This Line Gas An Error Check It Out In The Future..
            //std::vector<Ermine::MutexCasket> LockCasket = std::move(Casket->second);
            for (Ermine::MutexCasket& i : Casket->second)
            {
                Ermine::MutexLevel CasketLevel = i.GetLevelNumber().value_or(Ermine::MutexLevel::Empty);

                if (CasketLevel >= RecievedLevel)
                {
                    if (CasketLevel == RecievedLevel)
                        FlagToAvoidGettingMutex = true;
                    FlagToAvoidFlood = true;
                }
            }

            //If FLood Was Not Avoided.. 
            if (FlagToAvoidFlood == true)
            {
                if (FlagToAvoidGettingMutex == false)
                {
                    Casket->second.emplace_back(std::move(Ermine::MutexCasket(GetMutex(Obj->GetUniqueIdentifier(), (unsigned int)MutexInterface->GetMutexLevel()), MutexInterface->GetMutexLevel())));
                }
            }
            else
            {
                //The Casket Has Been Flooded
                Casket->second.clear();
                
                //Create A New Casket And Insert It Into The Storage Since One Does Not Exist..
                Casket->second.emplace_back(std::move(Ermine::MutexCasket(GetMutex(Obj->GetUniqueIdentifier(),(unsigned int)MutexInterface->GetMutexLevel()), MutexInterface->GetMutexLevel())));
            }
        }
        else
        {
            //Create A New Casket And Insert It Into The Storage Since One Does Not Exist..
            std::vector<Ermine::MutexCasket> LockCasket;
            LockCasket.emplace_back(std::move(Ermine::MutexCasket(GetMutex(Obj->GetUniqueIdentifier(), (unsigned int)MutexInterface->GetMutexLevel()), MutexInterface->GetMutexLevel())));//LockCasket.emplace_back(std::move(Ermine::MutexCasket(MutexInterface->GetUniqueLock(), MutexInterface->GetMutexLevel())));
            StorageBuffer[Obj->GetUniqueIdentifier()] = std::move(LockCasket);
        }
    }
    return GetMutexState(Obj->GetUniqueIdentifier());
}

void Ermine::MutexManager::ReleaseMutex(Ermine::Object* Obj, MutexLevel WhichMutex,Ermine::MutexState StateOfTheMutex)
{
    IMutex* MutexInterface = dynamic_cast<IMutex*>(Obj);
    Ermine::MutexLevel RecievedLevel = MutexInterface->GetMutexLevel();

    auto Casket = StorageBuffer.find(Obj->GetUniqueIdentifier());

    if (Casket != StorageBuffer.end())
    {
        //Casket Is Found..
        std::vector<Ermine::MutexCasket> LockCasket = std::move(Casket->second);
        for (auto& i = LockCasket.begin(); i < LockCasket.end();i++)//(auto& i : LockCasket)
        {
            Ermine::MutexLevel CasketLevel = i->GetLevelNumber().value_or(Ermine::MutexLevel::Empty);

            if (CasketLevel == RecievedLevel)
            {
                i = LockCasket.erase(i);
                break;
            }

            if (i == LockCasket.end())
                break;
        }
        StorageBuffer[Obj->GetUniqueIdentifier()] = std::move(LockCasket);
    }
    SetMutexState(Obj->GetUniqueIdentifier(), StateOfTheMutex);
}

std::unique_lock<std::recursive_mutex> Ermine::MutexManager::GetMutex(std::string ObjectName, unsigned int LevelNumber)
{
    return std::move(std::unique_lock<std::recursive_mutex>((MutexStorage[ObjectName]).second[LevelNumber]));
}

Ermine::MutexState Ermine::MutexManager::GetMutexState(std::string ObjectName)
{
    if (MutexStorage.find(ObjectName) == MutexStorage.end())
        return MutexState::Okay;
    else
        return (MutexStorage[ObjectName]).first;
}
void Ermine::MutexManager::SetMutexState(std::string ObjectName,Ermine::MutexState State)
{
    (MutexStorage[ObjectName]).first = State;
}
