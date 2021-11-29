#pragma once

#include "../command.hpp"
#include <vector>

class Command;

class Help : public Command
{
	public:
		Help(std::vector<Command*> &cmds);
		void execute();

	private:
		std::vector<Command*> &_cmds;
};