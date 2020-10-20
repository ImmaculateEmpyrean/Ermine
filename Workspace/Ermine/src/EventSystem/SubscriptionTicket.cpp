#include "stdafx.h"
#include "SubscriptionTicket.h"

Ermine::SubscriptionTicket::SubscriptionTicket(int Ticket,Ermine::EventType EventSubscriptionType)
    :
    Ticket(Ticket),
    EventType(EventSubscriptionType)
{}

Ermine::SubscriptionTicket::SubscriptionTicket(SubscriptionTicket&& rhs)
{
    this->Ticket = rhs.Ticket;
    rhs.Ticket = -1;
}

Ermine::SubscriptionTicket Ermine::SubscriptionTicket::operator=(SubscriptionTicket&& rhs)
{
    this->Ticket = rhs.Ticket;
    rhs.Ticket = -1;

    return std::move(*this);
}

int Ermine::SubscriptionTicket::GetTicket()
{
    return Ticket;
}

Ermine::EventType Ermine::SubscriptionTicket::GetEventSubscriptionType()
{
    return this->EventType;
}

Ermine::SubscriptionTicket::operator int()
{
    return Ticket;
}
