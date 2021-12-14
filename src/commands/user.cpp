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
	if(_args.size() > 1)
	{
		std::cout << "Là je suis dedans : " << _args[3] << _args[4] << std::endl;
		for (size_t i = 4; i < _args.size(); i++)
		{
			_user->setRealName(_user->getRealName() + _args[i]);
			if (i == 4)
				_user->getRealName().erase(0,1);
			if (i != _args.size())
				_user->getRealName() += " ";
			std::cout << _user->getRealName() << " :" << i << std::endl;
		}
		std::cout << "Done" << std::endl;
	}
}