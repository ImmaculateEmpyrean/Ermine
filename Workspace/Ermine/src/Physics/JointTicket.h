#pragma once
#include<iostream>
#include<vector>
#include<string>

#include<memory>

#include "box2d/b2_joint.h"

namespace Ermine
{
	//This Is Forwared Declared To Make It A Friend.. As It Should Only Be The One Issuing Tickets..
	class JointManager;

	class JointTicket
	{
	public:
		//No Reason For An Empty Ticket To Even Exist..
		JointTicket() = delete;

		//Two People Cant Have The Same Ticket Right...
		JointTicket(const JointTicket& rhs) = delete;
		JointTicket& operator = (const JointTicket& rhs) = delete;

		//Moving A Ticket Is Most Certainly Possible..
		JointTicket(JointTicket&& rhs);
		JointTicket operator = (JointTicket&& rhs);

	private:
		//This Is The Only Constructor To Be Used.. And Ticket Value Is Not To Be Medelled By The User At Any Cost..
		//Also Tickets Are Only Issued By The JointManger Not Whoever Wants To..
		JointTicket(int Ticket,b2JointType JointType);

	public:
		//Only The Getter Works.. Everybody Except The Station Must Only Be Able To See The Value Not Tamper With It..
		int GetTicket();

		//Check Which Subscription You Have.. Setter Not Available As Unintentional Forgery Will MostLikely Do You More Harm Than Good
		b2JointType  GetEventSubscriptionType();

		//Ticket Can Also Be Implicitly To Int.. Afterall Thats What It Really Is.. A Wrapper Around An Integer For Security Purposes..
		operator int();

	protected:

	protected:

	private:

	private:
		int Ticket;
		b2JointType JointType;

		//Only This Class Is Allowed To Legally Print Tickets
		friend class Ermine::JointManager;
	};
}