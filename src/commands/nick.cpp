#include "../../includes/commands/nick.hpp"
#include <iostream>

Nick::Nick(User *user) : Command("NICK", "<nickname> : Change client nickname.", user)
{
}

void Nick::execute()
{
	std::string nick = _user->getNick();

	if (_args.size() > 1 && _args[1] != ":")
	{
		std::string newNick = _args[1];
		User *user = _server->getUser(newNick);

		if (user != NULL && user != _user)
			throw ServerException::nickAlreadyUsed();

		if (newNick.size() > 0)
			_user->setNickName(newNick);
		else
			throw ServerException::invalidNick();
	}
}