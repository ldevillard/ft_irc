#include "../includes/Parser.hpp"

Parser::Parser(std::string line, ServData *data) : _line(line), _data(data)
{
	(void)_data;

	initCommands();

	if (isCommand())
		executeCommand();
}

void Parser::initCommands()
{
	_cmds_list.push_back(new Help());
	//push all commands
}

bool	Parser::isCommand()
{
	std::string fword = getFirstWord();

	std::vector<Command*>::iterator it;

	for(it = _cmds_list.begin(); it != _cmds_list.end(); it++)
	{
		if ((*it)->getName() == fword)
			return true;
	}

	return false;
}

std::string Parser::getFirstWord()
{
	std::string str;

	int i = 0;
	while (_line[i] && _line[i] == ' ') //Eventually all the whitespaces
		i++;
	while (_line[i] && _line[i] != ' ')
		str += _line[i++];
	return str;
}

void	Parser::executeCommand()
{
	std::string fword = getFirstWord();

	std::vector<Command*>::iterator it;

	for(it = _cmds_list.begin(); it != _cmds_list.end(); it++)
	{
		if ((*it)->getName() == fword)
			(*it)->execute();
	}
}