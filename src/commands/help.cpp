#include "../../includes/commands/help.hpp"

Help::Help(std::vector<Command*> &cmds) : Command("HELP", "Display all the commands and descriptions."), _cmds(cmds)
{
}

void Help::execute()
{
	std::string result;

	std::vector<Command*>::iterator it;

	for(it = _cmds.begin(); it != _cmds.end(); it++)
	{
		result += (*it)->getName();
		result += ": ";
		result += (*it)->getDesc();
		result += "\n";

	}

	std::cout << result; //send to server
}