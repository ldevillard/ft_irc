#pragma once

#include "../command.hpp"
#include "../user.hpp"
#include <vector>

class Command;

class Help : public Command
{
	public:
		Help(std::vector<Command*> &cmds, User *user);
		void execute();

	private:
		std::vector<Command*> &_cmds;
};