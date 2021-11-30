#include "../includes/command.hpp"

Command::Command(std::string name, std::string desc, User *user) : _name(name), _desc(desc), _user(user)
{
	_server = NULL;
}

Command::~Command()
{
}