#include "../../includes/commands/help.hpp"
#include "../../includes/rpl_codes.hpp"

Help::Help(std::vector<Command *> &cmds, Client *user) : Command("HELP", "HELP - Display all the commands and descriptions.", user), _cmds(cmds)
{
}

void Help::execute()
{
	std::vector<Command *>::iterator it;

	for (it = _cmds.begin(); it != _cmds.end(); it++)
		_user->sendMsg(":server " + std::string(RPL_HELPTXT) + " - " + (*it)->getDesc());
}