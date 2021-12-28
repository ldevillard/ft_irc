#include "../../includes/commands/notice.hpp"
#include "../../includes/rpl_codes.hpp"

Notice::Notice(Client *user) : Command("NOTICE", "NOTICE - <target> <text to be send>: Send message (without automatic reply) to other client or channel.", user)
{
}

std::string Notice::makeMessage()
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

void Notice::execute()
{
	std::string chanName = _args[1];

	if (chanName[0] == '#')
	{
		chanName.erase(chanName.begin());
		Channel *chan = _server->findChannel(_args[1]);
		if (chan != NULL)
		{
			std::string msg = makeMessage();
			chan->broadcastMsgExept(":" + _user->getNickName() + " NOTICE " + _args[1] + " :" + msg, _user);
		}
		else
		{
			std::string msg = makeMessage();
			chan = new Channel(_args[1]);
			_server->getChannels().insert(std::make_pair(_args[1], chan));
			chan->broadcastMsgExept(":" + _user->getNickName() + " NOTICE " + _args[1] + " :" + msg, _user);
			_server->getChannels().erase(chan->getName());
			delete chan;
		}
	}
	else
	{
		Client *targetUser = _server->getUser(chanName);
		if (!targetUser)
		{
			targetUser = new Client(_server);
			targetUser->setNickName(chanName);
			targetUser->sendMsg(":" + _user->getNickName() + "!" + _user->getNickName() + "@" + _user->getAddress() + " NOTICE " + chanName + " :" + makeMessage());
			delete targetUser;
		}
		else
			targetUser->sendMsg(":" + _user->getNickName() + "!" + _user->getNickName() + "@" + _user->getAddress() + " NOTICE " + chanName + " :" + makeMessage());
	}
}