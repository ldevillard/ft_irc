#include "../../includes/commands/privmsg.hpp"
#include <iostream>
#include "../../includes/channel.hpp"
#include "../../includes/rpl_codes.hpp"
#include "../../includes/message.hpp"
#include <vector>

Privmsg::Privmsg(Client *user) : Command("PRIVMSG", "<target> <text to be send>: Send message to other client or channel.", user)
{
}

std::string Privmsg::makeMessage()
{
    std::string ret;

    std::vector<std::string>::iterator it = _args.begin();
    it += 2;

    if ((*it)[0] == ':')
        (*it).erase(0, 1);
    while (it != _args.end())
    {
        ret += *it;
        if (it + 1 != _args.end())
            ret += " ";
        it++;
    }

    return ret;
}

void Privmsg::execute()
{
    //Manque les verifs
    if (_args[1] != "#")
    {
        Channel *chan = _server->findChannel(_args[1]);

        if (chan != NULL)
            chan->broadcastMsg(":" + _user->getNickName() + " PRIVMSG " + _args[1] + " :" + makeMessage());
    }
}