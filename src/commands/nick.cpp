#include "../../includes/commands/nick.hpp"
#include <iostream>

Nick::Nick(Client *user) : Command("NICK", "NICK - <nickname> : Change client nickname.", user)
{
}

bool Nick::charset(std::string &nick)
{
	std::string list = "#$%@&*!()\\|+~/?\'\"{}[];:";

	for (int i = 0; i < (int)nick.size(); i++)
	{
		for (int j = 0; j < (int)list.size(); j++)
		{
			if (nick[i] == list[j])
				return false;
		}
	}

	return true;
}

void Nick::execute()
{
	std::string nick = _user->getNickName();

	if (_args.size() > 1 && _args[1] != ":")
	{
		std::string newNick(_args[1]);
		Client *user = _server->getUser(newNick);

		if (user != NULL && user != _user)
		{
			_user->sendMsg(":server " + std::string(ERR_NOSUCHNICK) + " " + _args[1] + ": Nickname already used!");
			throw ServerException::nickAlreadyUsed();
		}
		if (newNick.size() > 0 && charset(newNick) == true)
		{
			if (_user->getNickName().size() > 0)
			{
				std::vector<Channel *> tab = _server->findChannelsOfUser(_user);
				for (std::vector<Channel *>::iterator it = tab.begin(); it != tab.end(); it++)
					(*it)->broadcastMsg(":" + _user->getNickName() + "!" + _user->getUserName() + "@" + _user->getAddress() + " NICK " + newNick);
			}
			_user->setNickName(newNick);
		}
		else
		{
			_user->sendMsg(":server " + std::string(ERR_ERRONEUSNICKNAME) + " " + _args[1] + ": Invalid Nickname!");
			throw ServerException::invalidNick();
		}
	}
}