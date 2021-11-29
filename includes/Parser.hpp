#pragma once

#include <vector>
#include <string>
#include <iostream>
#include "Command.hpp"
#include "Commands/Help.hpp"

class Parser
{
	public:
		Parser(std::string line);

		void	executeCommand();
		bool		isCommand();

	private:
		void initCommands();
		std::string getFirstWord();

		std::string _line;
		std::vector<Command*> _cmds_list;
};