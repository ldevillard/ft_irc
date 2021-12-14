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

		if (chan == NULL)
		{
			chan = new Channel(_args[1]);
			_server->getChannels().insert(std::make_pair(_args[1], chan));
		}

		chan->broadcastMsg(_args[1]);
		chan->join(_user);
	}
}