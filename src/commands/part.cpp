#include "../../includes/commands/part.hpp"
#include <iostream>
#include "../../includes/channel.hpp"
#include "../../includes/rpl_codes.hpp"
#include "../../includes/message.hpp"
#include <vector>

Part::Part(Client *user) : Command("PART", "PART - <channel> : Leave wanted channel", user)
{
}

void Part::execute()
{
    Channel *chan = _server->findChannel(_args[1]);
    if (chan == NULL)
        message::sendMsgToUser(_user, "Channel not exist!"); //Debug -> send correct message to user
    else if (chan->isUserInChannel(_user) == false)
        message::sendMsgToUser(_user, "You're not in channel"); //Debug -> send correct message to user
    else
    {
        //Leave Channel
        //Del channel if it's empty
    }
}