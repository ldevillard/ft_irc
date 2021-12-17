#pragma once

#include "../command.hpp"
#include "../client.hpp"
#include <vector>

class Command;

class Help : public Command
{
public:
	Help(std::vector<Command *> &cmds, Client *user);
	void execute();

private:
	std::vector<Command *> &_cmds;
};
