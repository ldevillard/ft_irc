#include "../includes/parser.hpp"
#include "../includes/commands/help.hpp"
#include "../includes/commands/join.hpp"
#include "../includes/commands/nick.hpp"
#include "../includes/commands/part.hpp"

Parser::Parser(std::string line, ServData *data, User *user) : _line(line), _data(data), _user(user)
{
	(void)_data;

	initCommands();

	if (isCommand())
	{
		try
		{
			executeCommand();
		}
		catch (std::exception const &e)
		{
			std::cout << "Error: " << e.what() << std::endl;
		}
	}
}

void Parser::initCommands()
{
	_cmds_list.push_back(new Help(_cmds_list, _user));
	_cmds_list.push_back(new Join(_user));
	_cmds_list.push_back(new Nick(_user));
	_cmds_list.push_back(new Part(_user));
	//push all commands

	std::vector<Command*>::iterator it;
	for(it = _cmds_list.begin(); it != _cmds_list.end(); it++)
		(*it)->setServer(_data);
}

bool	Parser::isCommand()
{
	std::vector<std::string> args = split(_line);

	std::vector<Command*>::iterator it;

	for(it = _cmds_list.begin(); it != _cmds_list.end(); it++)
	{
		if ((*it)->getName() == args[0])
			return true;
	}

	return false;
}

std::vector<std::string> Parser::split(std::string &line)
{
	std::vector<std::string> tab;
	std::string word_buf;

	for (int i = 0; line[i]; i++)
	{
		if (line[i] == ' ')
		{
			if (word_buf.size() > 0)
				tab.push_back(word_buf);
			word_buf.clear();
		}
		else if (line[i] != ' ')
			word_buf += line[i];
	}

	if (word_buf.size() > 0)
		tab.push_back(word_buf);
	return tab;
}

void	Parser::executeCommand()
{
	std::vector<std::string> args = split(_line);

	std::vector<Command*>::iterator it;

	for(it = _cmds_list.begin(); it != _cmds_list.end(); it++)
	{
		if ((*it)->getName() == args[0])
		{
			(*it)->setArgs(args);
			(*it)->execute();
		}
	}
}