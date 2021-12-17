#pragma once

#include <string>
#include <iostream>
#include "servData.hpp"
#include "serverException.hpp"

class Command
{
public:
	Command(std::string name, std::string desc, Client *user);
	virtual ~Command();

	std::string getDesc() const;
	std::string getName() const;

	void setServer(ServData *server);
	void setArgs(std::vector<std::string> &args);

	virtual void execute() = 0;

protected:
	std::string _name;
	std::string _desc;

	ServData *_server;
	Client *_user;
	std::vector<std::string> _args;
};

/* COMMANDS :
KICK
LIST
NICK
NOTICE
QUIT
WHO
*/