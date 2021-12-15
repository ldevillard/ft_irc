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
			throw ServerException::nickAlreadyUsed();
		if (newNick.size() > 0)
		{
			if (_user->getNickName().size() > 0)
				_user->sendMsg(":" + _user->getNickName() + "!" + _user->getUserName() + "@" + user->getAddress() + " NICK " + newNick);
			_user->setNickName(newNick);
			_user->setUserName(newNick);
		}
		else
			throw ServerException::invalidNick();
	}
}