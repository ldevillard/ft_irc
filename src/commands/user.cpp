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
	std::cout << "Là je suis dedans" << std::endl;
	if(_args.size() > 1 && _args[1] != ":")
	{
		std::cout << "Là je suis dedans : " << _args[4] << std::endl;
		if (_args[4].at(0) == ':')
		{
			std::cout << "First char is ok" << std::endl;
			if (_args.size() == 4)
				_user->setRealName(_args[4].erase(0,1));
			else
			{
				for (size_t i = 4; i < _args.size(); i++)
				{
					_user->setRealName(_user->getRealName() + _args[i]);
					if (i == 4)
						_user->getRealName().erase(0,1);
					std::cout << _user->getRealName() << std::endl;
				}
			}
		}
	}
}