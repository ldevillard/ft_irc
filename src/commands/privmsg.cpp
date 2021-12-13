#include "../../includes/commands/privmsg.hpp"
#include <iostream>
#include "../../includes/channel.hpp"
#include "../../includes/rpl_codes.hpp"
#include "../../includes/message.hpp"
#include <vector>

Privmsg::Privmsg(Client *user) : Command("PRIVMSG", "<target> <text to be send>: Send message to other client or channel.", user)
{
}

void Privmsg::execute()
{
    if (_args[1] != "#")
    {
        Channel *chan = _server->findChannel(_args[1]);

        if (chan != NULL)
            chan->broadcastMsg(":" + _user->getNickName() + " PRIVMSG " + _args[1] + " : " + _args[2]);
    }
}