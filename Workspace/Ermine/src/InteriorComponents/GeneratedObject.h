#pragma once
#include<iostream>

#include<vector>
#include<unordered_map>

#include<string>

#include<mutex>
#include<atomic>

#include<filesystem>
#include<functional>

#include "Log.h"
#include "EventSystem/EnumEventType.h"

#include "ObjectStatus.h"

namespace Ermine
{
	class Object;

	class Event;

	class SubscriptionTicket;

	class GeneratedObject
	{
	private:
		//Generated Object Is Managed And Used By The Object Class.. It Need Not Be.. No It Must Not Be Constructed Anywhere Else..
		GeneratedObject();

	public:
		//Start Since Generated Onject Must Always Be Safely Be Inside The HashTable.. We Wont Allow New Chains To Propogate.. By Allowing The Resource Itself To Be Copied.. Note-The Shared Pointer Can Still Be Comfortably Copied Or Moved Or Whatever//
		GeneratedObject(GeneratedObject& rhs) = delete;
		GeneratedObject& operator=(GeneratedObject& rhs) = delete;

		GeneratedObject(GeneratedObject&& rhs) = delete;
		GeneratedObject& operator=(GeneratedObject&& rhs) = delete;
		//Start Since Generated Onject Must Always Be Safely Be Inside The HashTable.. We Wont Allow New Chains To Propogate.. By Allowing The Resource Itself To Be Copied.. Note-The Shared Pointer Can Still Be Comfortably Copied Or Moved Or Whatever//

	public:
		std::string GetUniqueIdentifier() { return UniqueIdentifier; }

		Ermine::ObjectStatus GetObjectHealth();

		void SetObjectHealth(Ermine::ObjectStatus Status);
		void MarkObjectForDeletion();
		void RestoreObjectHealth();//This Is A Dangerous Function.. Use With CAUTION 

		std::unique_lock<std::recursive_mutex> GetObjectMutex() { return std::move(std::unique_lock<std::recursive_mutex>(ObjectMutex)); };

		//Start Event Subscriptions//
		void EventReciever(Ermine::Event* Eve);

		//Use This Method To Set Which Events Must Be Recieved By The Object..
		void RecieveEvents(bool Recieve, Ermine::EventType Type);

		bool IsRecievingConcreteEvents() { return FlagsOfRecievingEvents[(unsigned int)Ermine::EventType::ConcreteEvent]; }
		bool IsRecievingKeyCallbackEvents() { return FlagsOfRecievingEvents[(unsigned int)Ermine::EventType::KeyCallbackEvent]; }
		bool IsRecievingCharacterKeyCallbackEvents() { return FlagsOfRecievingEvents[(unsigned int)Ermine::EventType::CharacterCallbackEvent]; }
		bool IsRecievingCursorPositionUpdateEvents() { return FlagsOfRecievingEvents[(unsigned int)Ermine::EventType::CursorPositionCallbackEvent]; };
		bool IsRecievingMouseButtonCallbackEvents() { return FlagsOfRecievingEvents[(unsigned int)Ermine::EventType::MouseButtonCallbackEvent]; };
		bool IsRecievingScrollUpdateEvents() { return FlagsOfRecievingEvents[(unsigned int)Ermine::EventType::ScrollCallbackEvent]; }
		
		bool IsRecievingTileSelectedCallbackEvents() { return FlagsOfRecievingEvents[(unsigned int)Ermine::EventType::TileSelectedEvent]; };
		
		bool IsRecievingOnTickEvents() { return FlagsOfRecievingEvents[(unsigned int)Ermine::EventType::OnTickEvent]; };
		bool IsRecievingOnBeginEvents() { return FlagsOfRecievingEvents[(unsigned int)Ermine::EventType::OnBeginEvent]; };
		
		//Recieve All Flags As A Vector Of Bool Instead Of Specified Singular Bool
		std::vector<bool> AllFlagsOfRecievingEvents() { return FlagsOfRecievingEvents; }

#pragma region BindAndUnbindOBjects
	public:
		//Use This Function To Bind An Object To This Generated Object..
		void BindObject(Ermine::Object* Handle);

		//Unbind The GeneratedObject From The Object Prime..
		void UnBindObject();

		//Is Handle Valid Returns A Flag To U Telling If The Generated Object Thinks The Object Is Valid.
		bool IsHandleValid() { return HandleValid; };

		//DONOT USE THIS Function NOT AT ALL RECOMMENDED May Lead To Calling Whatever Is Being Pointed By Handle Even If It Is Not A Function. May Lead To Disaster Of An Inprecedented Scale..
		void SetHandleValidForce() { HandleValid = true; }; 

#pragma endregion

	public:

	protected:

	protected:

	private:
		//This Function Just Tries Printing DebugInfo If Something Goes Wrong..
		void DefaultEventHandler() { STDOUTDefaultLog_Critical("Default Handler Called"); };

	private:
		static std::atomic<long> Counter;
		std::string UniqueIdentifier;

		//ObjectStatus ObjectHealth = Ermine::ObjectStatus::StatusOk;

		//Flags To Check And Tickets To Store If A Certain Event Is Executing..
		std::function<void(std::string)> ConcreteEventFunctionPointer = nullptr;
		std::function<void(int, int, int, int)> KeyCallBackEventFunctionPointer = nullptr;
		std::function<void(int)> CharacterCallbackEventFunctionPointer = nullptr;
		std::function<void(int, int)> CursorPositionUpdateEventFunctionPointer = nullptr;
		std::function<void(int, int, int)> MouseButtonCallbackFunctionPointer = nullptr;
		std::function<void(double, double)>ScrollPositionUpdateFunctionPointer = nullptr;
		std::function<void(float)> OnTickEventFunctionPointer = nullptr;
		std::function<void(std::filesystem::path, int)> TileSelectedUpdateFunctionPointer = nullptr;

		std::vector<bool> FlagsOfRecievingEvents;
		std::vector<Ermine::SubscriptionTicket*> TicketsHeldByTheObject;

		std::atomic<bool> ObjectReadyToRecieveEvents = true;

		std::recursive_mutex ObjectMutex;

		//Handle To The Object.. 
		Ermine::Object* HObject = nullptr;

		//Will Pass On Events And Other Things To Object If This Flag Is Set To True.. Otherwise Will Not Pass On Events..
		bool HandleValid = false; 

		friend class Ermine::Object;
	};
}