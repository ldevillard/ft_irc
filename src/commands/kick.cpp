#include "../../includes/commands/kick.hpp"
#include "../../includes/rpl_codes.hpp"

Kick::Kick(Client *user) : Command("KICK", "KICK - <user> kick a user on a channel (op command).", user)
{
}

void Kick::execute()
{
	if (_args.size() < 3)
		return;
	Channel *chan = _server->findChannel(_args[1]);
	if (chan == NULL)
		_user->sendMsg(":server " + std::string(ERR_NOSUCHCHANNEL) + " " + _args[1] + ": No such channel!");
	else
	{
		if (chan->isUserInChannel(_user) == false)
			_user->sendMsg(":server " + std::string(ERR_USERNOTINCHANNEL) + " " + _args[1] + ": You're not in such channel!");
		else if (chan->isOp(_user) == false)
			_user->sendMsg(":server " + std::string(ERR_CHANOPRIVSNEEDED) + " " + _args[1] + ": You're not an operator of this channel!");
		else
		{
			Client *target = chan->findUserWithName(_args[2]);

			if (target == NULL)
				_user->sendMsg(":server " + std::string(ERR_USERNOTINCHANNEL) + " " + _args[1] + ": Can't find target in channel!");
			else
			{
				std::string message;
				for (std::vector<std::string>::iterator it = _args.begin() + 3; it != _args.end(); it++)
				{
					message += (*it);
					if (it + 1 != _args.end())
						message += " ";
				}
				chan->broadcastMsg(":" + _user->getNickName() + " KICK " + _args[1] + " " + target->getNickName() + " " + message);
				chan->leave(target);
			}
		}
	}
}
