#pragma once
#include "MailBox.h"

struct ActorRef 
{
    std::shared_ptr<Mailbox> _mailbox;
    std::string _name;

    void Tell(Message m) const 
    {
        _mailbox->Push(std::move(m));
    }
};