#include "stdafx.h"
#include "MutexManager.h"

#include "Interfaces/IMutex.h"

//Start Static Variables Declarations..//
thread_local Ermine::MutexManager* Ermine::MutexManager::Manager = nullptr;
thread_local std::once_flag Ermine::MutexManager::InitializationFlag;
//Ended Static Variables Declarations..//

Ermine::MutexManager* Ermine::MutexManager::GetManager()
{
    std::call_once(InitializationFlag, []() {
        Manager = new MutexManager();
    });

    return Manager;
}

void Ermine::MutexManager::AcquireMutex(Ermine::Object* Obj, MutexLevel WhichMutex)
{
    bool FlagToAvoidFlood = false;

    //Please Implement Some Kind Of Check Atleast In The Debug Mode In The Future..
    IMutex* MutexInterface = (IMutex*)Obj; 
    Ermine::MutexLevel RecievedLevel = MutexInterface->GetMutexLevel();

    auto Casket = StorageBuffer.find(Obj->GetUniqueIdentifier());

    if (Casket != StorageBuffer.end())
    {
        //Casket Is Found.. //Maybe This Line Gas An Error Check It Out In The Future..
        std::vector<Ermine::MutexCasket>& LockCasket = Casket->second;
        for (Ermine::MutexCasket& i : LockCasket)
        {
           Ermine::MutexLevel CasketLevel = i.GetLevelNumber().value_or(Ermine::MutexLevel::Empty);

           if (CasketLevel >= RecievedLevel)
               FlagToAvoidFlood = true;
        }

        //If FLood Was Not Avoided.. The Destructor Of The Vector Of Caskets Will Loose The Mutex You So Desperately Want To Use.. The Caskets Get Destroyed When They Go Out Of Scope At Function End..
        if (FlagToAvoidFlood == true)
        {
            LockCasket.emplace_back(std::move(Ermine::MutexCasket(MutexInterface->GetUniqueLock(), MutexInterface->GetMutexLevel())));
            StorageBuffer.emplace(Obj->GetUniqueIdentifier(), std::move(LockCasket));
        }
        else
        {
            //The Casket Has Been Flooded
            //Create A New Casket And Insert It Into The Storage Since One Does Not Exist..
            std::vector<Ermine::MutexCasket> LockCasket;
            LockCasket.emplace_back(std::move(Ermine::MutexCasket(MutexInterface->GetUniqueLock(), MutexInterface->GetMutexLevel())));
            StorageBuffer[Obj->GetUniqueIdentifier()] = std::move(LockCasket);
        }
    }
    else
    {
        //Create A New Casket And Insert It Into The Storage Since One Does Not Exist..
        std::vector<Ermine::MutexCasket> LockCasket;
        LockCasket.emplace_back(std::move(Ermine::MutexCasket(MutexInterface->GetUniqueLock(),MutexInterface->GetMutexLevel())));
        StorageBuffer[Obj->GetUniqueIdentifier()] = std::move(LockCasket);
    }
}

void Ermine::MutexManager::ReleaseMutex(Ermine::Object* Obj, MutexLevel WhichMutex)
{
    IMutex* MutexInterface = (IMutex*)Obj;
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
                i = LockCasket.erase(i);
        }

        StorageBuffer[Obj->GetUniqueIdentifier()] = std::move(LockCasket);
    }
}
