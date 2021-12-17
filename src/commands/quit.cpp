#include "../../includes/commands/quit.hpp"
#include "../../includes/rpl_codes.hpp"

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

	if (_args.size() > 1 && (_args[1] != ":" && _args.size() == 2)) //Send msg to let channels know that user disconnected
	{
		for (std::vector<Channel *>::iterator it = chans.begin(); it != chans.end(); it++)
			(*it)->broadcastMsg(":" + _user->getNickName() + " PRIVMSG " + (*it)->getName() + " :" + makeMessage());
	}

	for (std::vector<Channel *>::iterator it = chans.begin(); it != chans.end(); it++) //Leave all chanels
	{
		(*it)->broadcastMsg(":" + _user->getNickName() + " QUIT :");
		(*it)->leave(_user);
	}

	_user->sendMsg(":127.0.0.1 " + std::string(RPL_ENDOFLINKS) + " " + _args[0] + ": Quiting...");
	//Maybe close user fd to disconnect quickly ?
}