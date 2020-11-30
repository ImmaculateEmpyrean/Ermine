#include "stdafx.h"
#include "JointTicket.h"

namespace Ermine
{
    JointTicket::JointTicket(int Ticket, b2JointType JointType)
        :
        Ticket(Ticket),
        JointType(JointType)
    {}

    JointTicket::JointTicket(JointTicket&& rhs)
    {
        this->Ticket = std::move(rhs.Ticket);
        rhs.Ticket = -1;

        JointType = rhs.JointType;
    }

    JointTicket Ermine::JointTicket::operator=(JointTicket&& rhs)
    {
        this->Ticket = rhs.Ticket;
        rhs.Ticket = -1;

        JointType = rhs.JointType;

        return std::move(*this);
    }

    int JointTicket::GetTicket()
    {
        return Ticket;
    }

    b2JointType JointTicket::GetEventSubscriptionType()
    {
        return this->JointType;
    }

    JointTicket::operator int()
    {
        return Ticket;
    }
}
