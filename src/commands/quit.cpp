#include "../../includes/commands/quit.hpp"
#include "../../includes/rpl_codes.hpp"
#include "../../server_config.hpp"

Quit::Quit(Client *user) : Command("QUIT", "QUIT - Disconnect from the server.", user)
{
}

std::string Quit::makeMessage()
{
	std::string ret;
	std::vector<std::string>::iterator it = _args.begin();

	it++;
	if ((*it)[0] == ':')
		(*it).erase(0, 1);
	while (it != _args.end())
	{
		ret += *it;
		if (it + 1 != _args.end())
			ret += " ";
		it++;
	}

	return ret;
}

void Quit::execute()
{
	std::vector<Channel *> chans = _server->findChannelsOfUser(_user);

	if (_args.size() > 1 && (_args[1] != ":")) //Send msg to let channels know that user disconnected
	{
		for (std::vector<Channel *>::iterator it = chans.begin(); it != chans.end(); it++)
		{
			(*it)->broadcastMsgExept(":" + _user->getNickName() + "!" + _user->getUserName() + "@" + _user->getAddress() + " QUIT :" + makeMessage(), _user);
			(*it)->leave(_user, false);
		}
	}
	else
		for (std::vector<Channel *>::iterator it = chans.begin(); it != chans.end(); it++) //Leave all chanels
		{
			(*it)->broadcastMsgExept(":" + _user->getNickName() + "!" + _user->getUserName() + "@" + _user->getAddress() + " QUIT :" + CFG_DEFAULTQUITMSG, _user);
			(*it)->leave(_user, false);
		}
	_user->setKill(true);
}
