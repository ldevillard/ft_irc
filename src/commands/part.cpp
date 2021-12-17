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
	if (_args[1] != "#")
	{
		Channel *chan = _server->findChannel(_args[1]);

		if (chan != NULL)
		{
			if (chan->isUserInChannel(_user) == true)
			{
				chan->leave(_user);
				if (chan->isEmpty() == true)
                {
					_server->getChannels().erase(chan->getName());
                    delete chan;
                }
			}
			else
				_user->sendMsg(":server " + std::string(ERR_USERNOTINCHANNEL) + " " + _args[1] + ": You're not in such channel!");
		}
		else
			_user->sendMsg(":server " + std::string(ERR_NOSUCHCHANNEL) + " " + _args[1] + ": No such channel!");
	}
}
