#include "../includes/parser.hpp"
#include "../includes/commands/help.hpp"
#include "../includes/commands/join.hpp"
#include "../includes/commands/nick.hpp"
#include "../includes/commands/part.hpp"
#include "../includes/commands/user.hpp"
#include "../includes/commands/die.hpp"
#include "../includes/rpl_codes.hpp"
#include "../includes/commands/privmsg.hpp"

Parser::Parser(std::string line, ServData *data, Client *user) : _line(line), _data(data), _user(user)
{
	(void)_data;
	std::vector<std::string> args = split(_line);

	initCommands();

	if (isCommand())
	{
		if (!user->isRegistered())
		{
			//std::cout << "new command " << args[0] << std::endl;
			if (args[0] == "NICK" || args[0] == "USER")
			{
				try
				{
					executeCommand();
				}
				catch (std::exception const &e)
				{
					std::cout << "Error: " << e.what() << std::endl;
				}
				if (!user->getNickName().empty() && !user->getUserName().empty())
				{
					user->setRegistered(true);
					user->sendMsg(":127.0.0.1 " + std::string(RPL_WELCOME) + " " + user->getNickName() + " :Welcome to the Internet Relay Network " + user->getNickName() + "!" + user->getUserName() + "@" + user->getAddress());
				}
			}
			else
			{
				user->sendMsg(std::string(ERR_NOTREGISTERED) + ":You have not registered");
			}
		}
		else
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
}

Parser::~Parser()
{
	std::vector<Command *>::iterator it;

	for (it = _cmds_list.begin(); it != _cmds_list.end(); it++)
		delete *it;
}

void Parser::initCommands()
{
	_cmds_list.push_back(new Help(_cmds_list, _user));
	_cmds_list.push_back(new Join(_user));
	_cmds_list.push_back(new Nick(_user));
	_cmds_list.push_back(new Part(_user));
	_cmds_list.push_back(new User(_user));
	_cmds_list.push_back(new Privmsg(_user));
	_cmds_list.push_back(new Die(_user));
	//push all commands

	std::vector<Command *>::iterator it;
	for (it = _cmds_list.begin(); it != _cmds_list.end(); it++)
		(*it)->setServer(_data);
}

bool Parser::isCommand()
{
	std::vector<std::string> args = split(_line);

	std::vector<Command *>::iterator it;

	for (it = _cmds_list.begin(); it != _cmds_list.end(); it++)
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

void Parser::executeCommand()
{
	std::vector<std::string> args = split(_line);

	std::vector<Command *>::iterator it;

	for (it = _cmds_list.begin(); it != _cmds_list.end(); it++)
	{
		if ((*it)->getName() == args[0])
		{
			(*it)->setArgs(args);
			(*it)->execute();
		}
	}
}