#include "../../includes/commands/privmsg.hpp"
#include <iostream>
#include "../../includes/channel.hpp"
#include "../../includes/rpl_codes.hpp"
#include "../../includes/message.hpp"
#include <vector>
#include "../../includes/B_O_A_T.hpp"
#include "../../server_config.hpp"

Privmsg::Privmsg(Client *user) : Command("PRIVMSG", "PRIVMSG - <target> <text to be send>: Send message to other client or channel.", user)
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
	std::string chanName = _args[1];

	if (chanName.empty())
		_user->sendMsg(":server " + std::string(ERR_NOSUCHCHANNEL) + " " + _args[1] + ": No such channel!");
	else if (chanName[0] == '#')
	{
		chanName.erase(chanName.begin());
		if (chanName.empty())
			_user->sendMsg(":server " + std::string(ERR_NOSUCHCHANNEL) + " " + _args[1] + ": No such channel!");
		else
		{
			Channel *chan = _server->findChannel(_args[1]);
			if (chan != NULL)
			{
				std::string msg = makeMessage();
				if (chan->isUserInChannel(_user))
				{
					if (boating(msg, _user, chan))
						chan->broadcastMsgExept(":" + _user->getNickName() + " PRIVMSG " + _args[1] + " :" + msg, _user);
				}
				else
					_user->sendMsg(":server " + std::string(ERR_USERNOTINCHANNEL) + " " + _args[1] + ": You're not in such channel!");
			}
			else
				_user->sendMsg(":server " + std::string(ERR_NOSUCHCHANNEL) + " " + _args[1] + ": No such channel!");
		}
	}
	else
	{
		Client *targetUser = _server->getUser(chanName);
		if (!targetUser)
			_user->sendMsg(":server " + std::string(ERR_NOSUCHNICK) + " " + _args[1] + ": No such nick!");
		else
			targetUser->sendMsg(":" + _user->getNickName() + "!" + _user->getNickName() + "@" + _user->getAddress() + " PRIVMSG " + targetUser->getNickName() + " :" + makeMessage());
	}
}
