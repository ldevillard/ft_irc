#pragma once

#include <vector>
#include <string>
#include <iostream>
#include "Command.hpp"
#include "Commands/Help.hpp"
#include "servData.hpp"

class Parser
{
	public:
		Parser(std::string line, ServData *data);

		void	executeCommand();
		bool		isCommand();

	private:
		void initCommands();
		std::string getFirstWord();

		std::string _line;
		std::vector<Command*> _cmds_list;

		ServData *_data;
};