#pragma once
#include<iostream>
#include<vector>
#include<string>

#include "EnumEventType.h"

namespace Ermine
{
	//Forward Declaration So That Subscription Ticket Class Can Be A Friend Of EventBroadcastStation Allowing The Station To Print Tickets Using The Private Constructor :>
	class EventBroadcastStation;

	class SubscriptionTicket
	{
	public:
		//No Reason For An Empty Ticket To Even Exist..
		SubscriptionTicket() = delete; 

		//Two People Cant Have The Same Ticket Right...
		SubscriptionTicket(const SubscriptionTicket& rhs) = delete;
		SubscriptionTicket& operator = (const SubscriptionTicket& rhs) = delete;

		//Moving A Ticket Is Most Certainly Possible..
		SubscriptionTicket(SubscriptionTicket&& rhs);
		SubscriptionTicket operator = (SubscriptionTicket&& rhs);

	private:
		//This Is The Only Constructor To Be Used.. And Ticket Value Is Not To Be Medelled By The User At Any Cost..
		//Also Tickets Are Only Issued By The BroadcastingStation Not Whoever Wants To..
		SubscriptionTicket(int Ticket,Ermine::EventType EventSubscriptionType);

	public:
		//Only The Getter Works.. Everybody Except The Station Must Only Be Able To See The Value Not Tamper With It..
		int GetTicket();

		//Check Which Subscription You Have.. Setter Not Available As Unintentional Forgery Will MostLikely Do You More Harm Than Good
		Ermine::EventType GetEventSubscriptionType();

		//Ticket Can Also Be Implicitly To Int.. Afterall Thats What It Really Is.. A Wrapper Around An Integer For Security Purposes..
		operator int(); 

	protected:

	protected:

	private:

	private:
		int Ticket;
		Ermine::EventType EventType;

		//Only This Class Is Allowed To Legally Print Tickets
		friend class Ermine::EventBroadcastStation;
	};
}