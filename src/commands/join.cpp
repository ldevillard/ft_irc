#include "../../includes/commands/join.hpp"
#include <iostream>
#include "../../includes/channel.hpp"
#include "../../includes/rpl_codes.hpp"
#include "../../includes/message.hpp"

Join::Join(Client *user) : Command("JOIN", "JOIN - <channel> <password> : Join wanted channel.", user)
{
}

void Join::execute()
{

	if (_args[1] != "#")
	{
		Channel *chan = _server->findChannel(_args[1]);

		if (!chan)
		{
			chan = new Channel(_args[1]);
			_server->getChannels().insert(std::make_pair(_args[1], chan));
		}
		if (!chan->isUserInChannel(_user))
		{
			chan->broadcastMsg(_args[1]);
			if (chan->getMembers().empty())
				chan->setOp(_user, true);
			chan->join(_user);
		}
		else
		{
			_user->sendMsg(":server " + std::string(ERR_USERONCHANNEL) + +" " + _user->getNickName() + " " + chan->getName() + " :You are already on this channel");
		}
	}
}