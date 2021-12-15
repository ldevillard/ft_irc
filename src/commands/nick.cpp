#include "../../includes/commands/nick.hpp"
#include <iostream>

Nick::Nick(Client *user) : Command("NICK", "NICK - <nickname> : Change client nickname.", user)
{
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
			_user->sendMsg(":127.0.0.1 " + std::string(ERR_NOSUCHNICK) + " " + _args[1] + ": Nickname already used!");
			throw ServerException::nickAlreadyUsed();
		}
		if (newNick.size() > 0)
		{
			if (_user->getNickName().size() > 0)
			{
				message::sendMsgToUser(_user, ":" + _user->getNickName() + "!" + _user->getUserName() + "@" + _user->getAddress() + " NICK " + newNick);
				std::vector<Channel*> tab = _server->findChannelsOfUser(_user);
				std::vector<Channel*>::iterator it = tab.begin();
				while(it != tab.end())
				{
					(*it)->broadcastMsg(":" + _user->getNickName() + "!" + _user->getUserName() + "@" + _user->getAddress() + " NICK " + newNick);
					it++;
				}
			}
			_user->setNickName(newNick);
		}
		else
		{
			_user->sendMsg(":127.0.0.1 " + std::string(ERR_ERRONEUSNICKNAME) + " " + _args[1] + ": Invalid Nickname!");
			throw ServerException::invalidNick();
		}
	}
}