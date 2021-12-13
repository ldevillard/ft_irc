#include "../../includes/commands/privmsg.hpp"
#include <iostream>
#include "../../includes/channel.hpp"
#include "../../includes/rpl_codes.hpp"
#include "../../includes/message.hpp"
#include <vector>

Privmsg::Privmsg(User *user) : Command("PRIVMSG", "<target> <text to be send>: Send message to other client or channel.", user)
{
}

void Privmsg::execute()
{
    /*if (_args[1] != "#")
    {
        Channel *chan = _server->findChannel(_args[1]);

        if (chan != NULL)
            chan->broadcastMsg("No way dude !");
    }*/
}