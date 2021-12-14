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
	if(_args.size() > 1)
	{
		if (_args.size() > 5)
			_args[4].erase(0,1);
		for (size_t i = 4; i < _args.size(); i++)
		{
			_user->setRealName(_user->getRealName() + _args[i]);
			if (i != _args.size() - 1)
				_user->getRealName() += " ";
		}
	}
}