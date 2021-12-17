#include "../includes/command.hpp"

Command::Command(std::string name, std::string desc, Client *user) : _name(name), _desc(desc), _user(user)
{
	_server = NULL;
}

Command::~Command()
{
}

std::string Command::getDesc() const { return _desc; }
std::string Command::getName() const { return _name; }

void Command::setServer(ServData *server) { _server = server; }
void Command::setArgs(std::vector<std::string> &args) { _args = args; }
