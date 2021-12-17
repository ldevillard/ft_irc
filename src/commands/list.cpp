#include "../../includes/commands/list.hpp"
#include "../../includes/rpl_codes.hpp"

List::List(Client *user) : Command("LIST", "LIST - Show all channels on the server.", user)
{
}

std::string itoa(int nb)
{
	std::ostringstream s;
	s << nb;
	std::string str(s.str());
	return str;
}

void List::execute()
{
	std::map<std::string, Channel *>::iterator it;

	if (_args.size() > 1)
	{
		Channel *chan = _server->findChannel(_args[1]);
		if (chan == NULL)
			_user->sendMsg(":server " + std::string(ERR_NOSUCHCHANNEL) + " " + _args[1] + ": No such channel!");
		else
		{
			_user->sendMsg(":server " + std::string(RPL_LIST) + " * " + (*it).second->getName() + " " + itoa((*it).second->getMembers().size())); //TO STRING
		}
	}
	else
	{
		for (it = _server->getChannels().begin(); it != _server->getChannels().end(); it++)
		{
			_user->sendMsg(":server " + std::string(RPL_LIST) + " * " + (*it).second->getName() + " " + itoa((*it).second->getMembers().size())); //TO STRING
		}
	}
	_user->sendMsg(":server " + std::string(RPL_LISTEND) + " : End of LIST");
}