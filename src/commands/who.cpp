#include "../../includes/commands/who.hpp"
#include "../../includes/rpl_codes.hpp"

std::string itoa(int nb);

Who::Who(Client *user) : Command("WHO", "WHO - <channel/user> List users", user)
{
}

void Who::execute()
{
	if (_args.size() == 1) //LIST ALL USERS ON SERVER
	{
		std::vector<Client *> users = _server->getVectorUser();

		if (users.size() == 1)
			_user->sendMsg(":server " + std::string(RPL_WHOREPLY) + " Server has : " + itoa(users.size()) + " user.");
		else if (users.size() > 1)
			_user->sendMsg(":server " + std::string(RPL_WHOREPLY) + " Server has : " + itoa(users.size()) + " users.");

		for (std::vector<Client *>::iterator it = users.begin(); it != users.end(); it++)
		{
			_user->sendMsg(":server " + std::string(RPL_WHOREPLY) + " " + (*it)->getNickName() + " " + (*it)->getUserName() + "@" + (*it)->getAddress());
		}
		_user->sendMsg(":server " + std::string(RPL_ENDOFWHO) + " : End of WHO");
	}
	else
	{
		Channel *chan = _server->findChannel(_args[1]);
		if (chan == NULL)
			_user->sendMsg(":server " + std::string(ERR_NOSUCHCHANNEL) + " " + _args[1] + ": No such channel!");
		else
		{
			std::vector<Client *> users = chan->getMembers();

			if (users.size() == 1)
				_user->sendMsg(":server " + std::string(RPL_WHOREPLY) + " Channel " + _args[1] + " has : " + itoa(users.size()) + " user.");
			else if (users.size() > 1)
				_user->sendMsg(":server " + std::string(RPL_WHOREPLY) + " Channel " + _args[1] + " has : " + itoa(users.size()) + " users.");

			for (std::vector<Client *>::iterator it = users.begin(); it != users.end(); it++)
			{
				std::string opCheck;
				if (chan->isOp(_user))
					opCheck += "@";
				_user->sendMsg(":server " + std::string(RPL_WHOREPLY) + " " + opCheck + (*it)->getNickName() + " " + (*it)->getUserName() + "@" + (*it)->getAddress());
			}
			_user->sendMsg(":server " + std::string(RPL_ENDOFWHO) + " : End of WHO");
		}
	}
}