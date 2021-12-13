#include "../../includes/commands/join.hpp"
#include <iostream>
#include "../../includes/channel.hpp"
#include "../../includes/rpl_codes.hpp"
#include "../../includes/message.hpp"

Join::Join(Client *user) : Command("JOIN", "<channel> <password> : Join wanted channel.", user)
{
}

void Join::execute()
{
	if (_args[1] != "#")
	{
		Channel *chan = new Channel(_args[1]);
		_server->getChannels().push_back(chan);

		chan->broadcastMsg(_args[1]);

		chan->join(_user);
	}
}