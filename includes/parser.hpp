#pragma once

#include <vector>
#include <string>
#include <iostream>
#include "command.hpp"
#include "servData.hpp"

class Parser
{
	public:
		Parser(std::string line, ServData *data, Client *user);
		~Parser();

		void	executeCommand();
		bool		isCommand();

	private:
		void initCommands();
		std::vector<std::string> split(std::string &line);

		std::string _line;
		std::vector<Command*> _cmds_list;

		ServData *_data;
		Client	*_user;
};
