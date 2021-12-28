#include "../../includes/commands/user.hpp"
#include <iostream>
#include "../../includes/channel.hpp"
#include "../../includes/rpl_codes.hpp"
#include "../../includes/message.hpp"

User::User(Client *user) : Command("USER", "USER - <username> <hostname> <servername> <realname> : Specify identity.", user)
{
}

void User::execute()
{
	if (_args.size() > 1)
	{
		if (_args.size() > 5)
			_args[4].erase(0, 1);
		for (size_t i = 4; i < _args.size(); i++)
		{
			_user->setRealName(_user->getRealName() + _args[i]);
			if (i != _args.size() - 1)
				_user->getRealName() += " ";
		}

		//MAKE VERIF
		if (_server->getUserFromUsername(_args[1]))
		{
			_user->sendMsg(":server " + std::string(ERR_NOSUCHNICK) + " " + _args[1] + ": Username already used!");
			throw ServerException::nickAlreadyUsed();
		}
		else
			_user->setUserName(_args[1]);
	}
}